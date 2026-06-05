#version 430 core


#define SAMPLES_PER_PIXEL 1
#define MAX_BOUNCES 5
#define EPSILON 0.0001f
#define SKY_COLOR vec3(0.0f, 0.0f, 0.0f)


struct Ray
{
	vec3 origin;
	vec3 dir;
};

struct RayPayload
{
	int materialIndex;
	float t;
	vec3 position;
	vec3 normal;
};

struct Material
{
	vec4 albedo; // RGB = albedo color, A = rougness
	vec4 emission; // RGB = emission color, A = strength
	vec4 params; // R = metallic, GBA = reserved for future use
};

struct BVHNode
{
	vec3 minBounds;
	uint leftFirst;
	vec3 maxBounds;
	uint triCount;
};

struct BVHTriangle
{
	vec4 v0; // .w is material index
	vec4 v1;
	vec4 v2;
};


in vec2 TexCoords;

layout (location = 0) out vec4 FragColor;


layout(std430, binding = 0) buffer ssbo4
{
	BVHNode b_BVHNodes[];
};

layout(std430, binding = 1) buffer ssbo1
{
    BVHTriangle b_BVHTriangles[];
};

layout(std430, binding = 2) buffer ssbo2
{
    Material b_Materials[];
};


uniform sampler2D u_ScreenTexture;
uniform uint u_AccumulationPasses;
uniform vec2 u_Resolution;
uniform uint u_FrameIndex;
uniform vec3 u_CameraPosition;
uniform mat4x4 u_CameraInverseProjection;
uniform mat4x4 u_CameraInverseView;


/**
 * REF: https://www.youtube.com/watch?v=5_RAHZQCPjE
 * REF: https://www.reedbeta.com/blog/hash-functions-for-gpu-rendering/
 */
uint pcg_hash(uint inp)
{
	uint state = inp * 747796405u + 2891336453u;
	uint word = ((state >> ((state >> 28u) + 4u)) ^ state) * 277803737u;
	return (word >> 22u) ^ word;
}

float random01(uint seed)
{
	uint hash = pcg_hash(seed);

	return float(hash) / 0xffffffffu;
}

float randomFloat(uint seed, float minf, float maxf)
{
	return random01(seed) * (maxf - minf) + minf;
}

vec2 randomVec2(uint seed, float minf, float maxf)
{
	return vec2(randomFloat(seed, minf, maxf), randomFloat(seed + 1, minf, maxf));
}

vec3 randomVec3(uint seed, float minf, float maxf)
{
	return vec3(randomFloat(seed, minf, maxf), randomFloat(seed + 1, minf, maxf), randomFloat(seed + 2, minf, maxf));
}

vec3 randomOnUnitSphere(uint seed)
{
	return normalize(randomVec3(seed, -1, 1));
}

vec3 cosineSampleHemisphere(uint seed, vec3 N)
{
	float u1 = random01(seed);
	float u2 = random01(seed + 1u);

	float r = sqrt(u1);
	float phi = 6.2831853f * u2;

	vec3 up = abs(N.z) < 0.999f ? vec3(0, 0, 1) : vec3(1, 0, 0);
	vec3 T = normalize(cross(up, N));
	vec3 B = cross(N, T);

	return normalize(T * (r * cos(phi)) + B * (r * sin(phi)) + N * sqrt(max(0.0f, 1.0f - u1)));
}

vec3 sampleGGXHalfVector(uint seed, vec3 N, float alpha)
{
	float u1 = random01(seed);
	float u2 = random01(seed + 1u);

	float phi = 6.2831853f * u1;
	float cosTheta = sqrt((1.0f - u2) / (1.0f + (alpha * alpha - 1.0f) * u2));
	float sinTheta = sqrt(max(0.0f, 1.0f - cosTheta * cosTheta));

	vec3 h = vec3(sinTheta * cos(phi), sinTheta * sin(phi), cosTheta);

	vec3 up = abs(N.z) < 0.999f ? vec3(0, 0, 1) : vec3(1, 0, 0);
	vec3 T = normalize(cross(up, N));
	vec3 B = cross(N, T);

	return normalize(T * h.x + B * h.y + N * h.z);
}

vec3 fresnelSchlick(float cosTheta, vec3 F0)
{
	return F0 + (1.0f - F0) * pow(clamp(1.0f - cosTheta, 0.0f, 1.0f), 5.0f);
}

// Exact Smith G1 masking term for GGX
float ggxSmithG1(float NoX, float alpha)
{
	float a2 = alpha * alpha;
	return 2.0f * NoX / (NoX + sqrt(a2 + (1.0f - a2) * NoX * NoX));
}


vec2 intersectAABB(vec3 rayOrigin, vec3 rayInvDir, vec3 boxMin, vec3 boxMax)
{
    vec3 t0 = (boxMin - rayOrigin) * rayInvDir;
    vec3 t1 = (boxMax - rayOrigin) * rayInvDir;

    vec3 tSmall = min(t0, t1);
    vec3 tBig = max(t0, t1);

    float tMin = max(max(tSmall.x, tSmall.y), tSmall.z);
    float tMax = min(min(tBig.x, tBig.y), tBig.z);

	// If tMax < 0, the box is completely behind the ray origin.
    // If tMin > tMax, the ray missed the box entirely.
    return vec2(tMin, tMax);
};

bool intersectTriangle(vec3 ro, vec3 rd, vec3 v0, vec3 v1, vec3 v2, out float t, out vec3 normal)
{
    const float eps = 1e-7;

    // Find vectors for two edges sharing v0
    vec3 edge1 = v1 - v0;
    vec3 edge2 = v2 - v0;

	vec3 ray_cross_edge2 = cross(rd, edge2);
	float det = dot(edge1, ray_cross_edge2);

	if (abs(det) < eps)
	{
		return false; // Ray is parallel to triangle plane
	}

	float inv_det = 1.0 / det;
	vec3 s = ro - v0;
	float u = inv_det * dot(s, ray_cross_edge2);

	if (u < -eps || u - 1.0 > eps)
	{
		return false; // Ray passes outside edge2's bounds
	}

	vec3 s_cross_e1 = cross(s, edge1);
    float v = inv_det * dot(rd, s_cross_e1);

	if (v < -eps || u + v - 1.0 > eps)
	{
		return false; // Ray passes outside edge1's bounds
	}

	t = inv_det * dot(edge2, s_cross_e1);

	if (t < eps)
	{
		return false; // Ray intersection is behind the ray origin
	}

    // Normal calculation. LH system.
	normal = normalize(cross(edge2, edge1));

	if (det > 0.0)
    {
        normal = -normal;
    }

	return true;
}


RayPayload traceRay(Ray ray)
{
	RayPayload payload;
	payload.materialIndex = -1;
	payload.t = 1e20f;

	vec3 invRayDir = 1.0f / ray.dir;

	uint stack[32];
	int stackPtr = 0;
	stack[stackPtr++] = 0; // Start with root node

	while (stackPtr > 0)
	{
		uint nodeIndex = stack[--stackPtr];
		BVHNode node = b_BVHNodes[nodeIndex];

		if (node.triCount > 0)
		{
			for (uint i = 0; i < node.triCount; i++)
			{
				uint triIndex = node.leftFirst + i;
				vec3 v0 = b_BVHTriangles[triIndex].v0.xyz;
				vec3 v1 = b_BVHTriangles[triIndex].v1.xyz;
				vec3 v2 = b_BVHTriangles[triIndex].v2.xyz;

				float tmp_t;
				vec3 tmp_normal;

				if (intersectTriangle(ray.origin, ray.dir, v0, v1, v2, tmp_t, tmp_normal))
				{
					if (tmp_t < payload.t)
					{
						payload.materialIndex = int(b_BVHTriangles[triIndex].v0.w);
						payload.t = tmp_t;
						payload.position = ray.origin + ray.dir * tmp_t;
						payload.normal = tmp_normal;
					}
				}
			}
		}
		else
		{
			uint left = node.leftFirst;
			uint right = node.leftFirst + 1;

			vec2 hitLeftData = intersectAABB(ray.origin, invRayDir, b_BVHNodes[left].minBounds, b_BVHNodes[left].maxBounds);
			vec2 hitRightData = intersectAABB(ray.origin, invRayDir, b_BVHNodes[right].minBounds, b_BVHNodes[right].maxBounds);

			bool hitLeft = hitLeftData.y >= 0 && hitLeftData.x <= hitLeftData.y;
			bool hitRight = hitRightData.y >= 0 && hitRightData.x <= hitRightData.y;

			if (hitLeft && hitLeftData.x > payload.t)
			{
				hitLeft = false; // No need to check this node if it's further than the closest hit so far
			}
			if (hitRight && hitRightData.x > payload.t)
			{
				hitRight = false; // No need to check this node if it's further than the closest hit so far
			}

			if (hitLeft && hitRight)
			{
				if (hitLeftData.x < hitRightData.x)
				{
					stack[stackPtr++] = right; // Further
					stack[stackPtr++] = left; // Closer
				}
				else
				{
					stack[stackPtr++] = left; // Further
					stack[stackPtr++] = right; // Closer
				}
			}
			else if (hitLeft)
			{
				stack[stackPtr++] = left;
			} 
			else if (hitRight)
			{
				stack[stackPtr++] = right;
			}
		}
	}

	return payload;
}

/**
 * Generation of rays - reflection
 *
 * REF: https://en.wikipedia.org/wiki/Ray_tracing_(graphics)#/media/File:Ray_Tracing_Illustration_First_Bounce.png
 * REF: https://www.scratchapixel.com/lessons/3d-basic-rendering/introduction-to-ray-tracing/adding-reflection-and-refraction.html
 * REF: https://www.scratchapixel.com/lessons/3d-basic-rendering/introduction-to-shading/reflection-refraction-fresnel.html
 * REF: https://raytracing.github.io/books/RayTracingInOneWeekend.html#dielectrics 
 */
vec3 rayGen(uint seed)
{
	vec2 rayOffset = randomVec2(seed, -0.5f, 0.5f);
	vec2 coord = (gl_FragCoord.xy + rayOffset) / u_Resolution.xy * 2.0f - 1.0f;
	vec4 target = u_CameraInverseProjection * vec4(coord.x, coord.y, 1, 1);
	vec3 viewDir = vec3(u_CameraInverseView * vec4(normalize(vec3(target) / target.w), 0));

	vec3 startPos = u_CameraPosition;
	vec3 direction = viewDir;

	vec3 light = vec3(0, 0, 0);
	vec3 contribution = vec3(1, 1, 1);

	for (int bounce = 0; bounce < MAX_BOUNCES; bounce++)
	{
		RayPayload payload = traceRay(Ray(startPos, direction));

		seed += bounce;
		seed = pcg_hash(seed);

		if (payload.materialIndex < 0)
		{
			light += contribution * SKY_COLOR;
			break;
		}
		else
		{
			Material mat = b_Materials[payload.materialIndex];

			vec3 albedo = mat.albedo.rgb;
			float roughness = clamp(mat.albedo.a, 0.02f, 1.0f); // avoid degenerate mirror spikes
			float metallic = mat.params.r;
			vec3 emissionColor = mat.emission.rgb;
			float emissionStrength = mat.emission.a;

			light += contribution * emissionColor * emissionStrength;

			vec3 N = payload.normal;
			vec3 V = -normalize(direction);
			float NoV = max(dot(N, V), 1e-4f);

			vec3 F0 = mix(vec3(0.04f), albedo, metallic); // Base reflectance at normal incidence. Dielectrics use ~0.04, metals use albedo color
			vec3 Fr = fresnelSchlick(NoV, F0);
			float pSpec = clamp(max(Fr.r, max(Fr.g, Fr.b)), 0.1f, 0.9f); // probability of sampling specular
			float alpha = roughness * roughness;

			seed = pcg_hash(seed + payload.materialIndex);

			vec3 newDir;

			if (random01(seed) < pSpec)
			{
				// Specular (GGX)
				seed = pcg_hash(seed);
				vec3 H = sampleGGXHalfVector(seed, N, alpha);
				newDir = reflect(direction, H);

				float NoL = dot(N, newDir);
				if (NoL <= 0.0f)
				{
					break; // sample went below the surface -> contributes nothing
				}

				float NoH = max(dot(N, H), 1e-4f);
				float VoH = max(dot(V, H), 1e-4f);
				vec3 F = fresnelSchlick(VoH, F0);
				float G = ggxSmithG1(NoV, alpha) * ggxSmithG1(NoL, alpha);

				// weight = BRDF * NoL / pdf  (NDF sampling pdf cancels D)
				contribution *= (F * G * VoH / (NoV * NoH)) / pSpec;
			}
			else
			{
				// Diffuse (lambertian)
				seed = pcg_hash(seed);
				newDir = cosineSampleHemisphere(seed, N);

				// cosine pdf cancels the cosine term -> weight is just albedo
				contribution *= albedo * (1.0f - metallic) / (1.0f - pSpec);
			}

			startPos = payload.position + N * EPSILON;
			direction = newDir;
		}

		// Russian roulette termination
		if (bounce > 3)
		{
			float p = max(contribution.r, max(contribution.g, contribution.b));

			seed += 1;
			seed = pcg_hash(seed);

			if (random01(seed) > p)
			{
				break;
			}
		}
	}

	return light;
}


/* 
 * Path Tracing implementation (SSAA)
 *
 * REF: https://raytracing.github.io/books/RayTracingInOneWeekend.html#antialiasing
 * REF: https://en.wikipedia.org/wiki/Supersampling
 * REF: https://en.wikipedia.org/wiki/Path_tracing
 * REF: https://www.youtube.com/watch?v=NIpC53vesHo
*/
vec3 samplePixel()
{
	vec3 result = vec3(0, 0, 0);
	uint seed = uint(gl_FragCoord.y * u_Resolution.x + gl_FragCoord.x) * u_FrameIndex;

	for (int i = 0; i < SAMPLES_PER_PIXEL; i++)
	{
		seed += i;
		seed = pcg_hash(seed);

		result += rayGen(seed);
	}

	return result / SAMPLES_PER_PIXEL;
}

vec4 accumulatePixel(vec4 color)
{
	float passes = float(u_AccumulationPasses);

	return (texture(u_ScreenTexture, TexCoords) * passes + color) / (passes + 1);
}


void main()
{
	FragColor = accumulatePixel(vec4(samplePixel(), 1.0f));
}