#version 430 core


#define SAMPLES_PER_PIXEL 1
#define MAX_BOUNCES 10
#define EPSILON 0.0001f


struct Ray
{
	vec3 origin;
	vec3 dir;
};

struct RayPayload
{
	int materialIndex;
	vec3 position;
	vec3 normal;
};

struct Material
{
	vec3 albedo;
	float roughness;
	float emission;
};

struct Sphere
{
	vec3 origin;
	float radius;
	int materialIndex;
};

struct Box
{
	vec3 boxMin;
	vec3 boxMax;
	int materialIndex;
};


in vec2 TexCoords;

layout (location = 0) out vec4 FragColor;


uniform sampler2D u_ScreenTexture;
uniform uint u_SamplesPerPixel;
uniform uint u_MaxBounces;
uniform uint u_AccumulationPasses;
uniform vec2 u_Resolution;
uniform uint u_FrameIndex;
uniform vec3 u_CameraPosition;
uniform mat4x4 u_CameraInverseProjection;
uniform mat4x4 u_CameraInverseView;


const Material MATERIALS[] = Material[]
(
	Material(vec3(1.0f, 0.0f, 0.0f), 1.0f, 0.0f),
	Material(vec3(0.0f, 1.0f, 0.0f), 1.0f, 0.0f),
	Material(vec3(1.0f, 1.0f, 1.0f), 1.0f, 0.0f),
	Material(vec3(1.0f, 1.0f, 1.0f), 0.0f, 0.0f),
	Material(vec3(1.0f, 1.0f, 1.0f), 1.0f, 5.0f)
);

const Sphere SPHERES[] = Sphere[]
(
	Sphere(vec3(0, -4, 2), 3, 3)
);

const Box BOXES[] = Box[]
(
	Box(vec3(-7, -7, -7), vec3(-6, 7, 7), 0),
	Box(vec3(6, -7, -7), vec3(7, 7, 7), 1),
	Box(vec3(-7, -7, 7), vec3(7, 7, 8), 2),
	Box(vec3(-7, 7, -7), vec3(7, 8, 7), 2),
	Box(vec3(-7, -7, -7), vec3(7, -8, 7), 2),
	Box(vec3(-1.5f, 5.75f, -1), vec3(1.5f, 6, 1), 4)
);


/**
 * REF: https://www.youtube.com/watch?v=5_RAHZQCPjE
 * REF: https://www.reedbeta.com/blog/hash-functions-for-gpu-rendering/
 */
uint pcg_hash(uint input)
{
	uint state = input * 747796405u + 2891336453u;
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

vec3 randomOnHemisphere(uint seed, vec3 normal)
{
	vec3 onUnitSphere = randomOnUnitSphere(seed);

	return sign(dot(onUnitSphere, normal)) * onUnitSphere;
}


RayPayload miss(Ray ray)
{
	RayPayload payload;
	payload.materialIndex = -1;

	return payload;
}

RayPayload closestHitSphere(Ray ray, float minT, int sphereIndex)
{
	RayPayload payload;
	payload.materialIndex = SPHERES[sphereIndex].materialIndex;
	payload.position = ray.origin + ray.dir * minT;
	payload.normal = (payload.position - SPHERES[sphereIndex].origin) / SPHERES[sphereIndex].radius;

	/**
	 * Check for backfaces
	 *
	 * REF: https://raytracing.github.io/books/RayTracingInOneWeekend.html#surfacenormalsandmultipleobjects/frontfacesversusbackfaces
	 * REF: https://www.scratchapixel.com/lessons/3d-basic-rendering/ray-tracing-rendering-a-triangle/single-vs-double-sided-triangle-backface-culling.html
	 */
	if (dot(ray.dir, payload.normal) > 0)
	{
		payload.normal = -payload.normal;
	}

	return payload;
}

RayPayload closestHitBox(Ray ray, float minT, int boxIndex)
{
	RayPayload payload;
	payload.materialIndex = BOXES[boxIndex].materialIndex;
	payload.position = ray.origin + ray.dir * minT;

	if (payload.position.x == min(BOXES[boxIndex].boxMin.x, BOXES[boxIndex].boxMax.x))
	{
		payload.normal = vec3(-1, 0, 0);
	}
	else if (payload.position.x == max(BOXES[boxIndex].boxMin.x, BOXES[boxIndex].boxMax.x))
	{
		payload.normal = vec3(1, 0, 0);
	}
	else if (payload.position.y == min(BOXES[boxIndex].boxMin.y, BOXES[boxIndex].boxMax.y))
	{
		payload.normal = vec3(0, -1, 0);
	}
	else if (payload.position.y == max(BOXES[boxIndex].boxMin.y, BOXES[boxIndex].boxMax.y))
	{
		payload.normal = vec3(0, 1, 0);
	}
	else if (payload.position.z == min(BOXES[boxIndex].boxMin.z, BOXES[boxIndex].boxMax.z))
	{
		payload.normal = vec3(0, 0, -1);
	}
	else if (payload.position.z == max(BOXES[boxIndex].boxMin.z, BOXES[boxIndex].boxMax.z))
	{
		payload.normal = vec3(0, 0, 1);
	}

	if (dot(ray.dir, payload.normal) > 0)
	{
		payload.normal = -payload.normal;
	}

	return payload;
}

RayPayload traceRay(Ray ray)
{
	float minTSphere = 99999999;
	int sphereIndex = -1;

	for (int i = 0; i < SPHERES.length(); i++)
	{
		vec3 origin = ray.origin - SPHERES[i].origin;

		float a = ray.dir.x * ray.dir.x + ray.dir.y * ray.dir.y + ray.dir.z * ray.dir.z;
		float half_b = origin.x * ray.dir.x + origin.y * ray.dir.y + origin.z * ray.dir.z;
		float c = origin.x * origin.x + origin.y * origin.y + origin.z * origin.z - SPHERES[i].radius * SPHERES[i].radius;

		float discriminant = half_b * half_b - a * c;

		if (discriminant < 0)
		{
			continue;
		}

		float t = (-half_b - sqrt(discriminant)) / a;

		if (t > 0)
		{
			if (t < minTSphere)
			{
				minTSphere = t;
				sphereIndex = i;
			}
		}
		else
		{
			t = (-half_b + sqrt(discriminant)) / a;

			if (t > 0)
			{
				if (t < minTSphere)
				{
					minTSphere = t;
					sphereIndex = i;
				}
			}
		}
	}

	float minTBox = 9999999;
	int boxIndex = -1;

	for (int i = 0; i < BOXES.length(); i++)
	{
		vec3 dirfrac = vec3(0, 0, 0);
		dirfrac.x = 1.0f / ray.dir.x;
		dirfrac.y = 1.0f / ray.dir.y;
		dirfrac.z = 1.0f / ray.dir.z;

		Box b = BOXES[i];

		float t1 = (b.boxMin.x - ray.origin.x) * dirfrac.x;
		float t2 = (b.boxMax.x - ray.origin.x) * dirfrac.x;
		float t3 = (b.boxMin.y - ray.origin.y) * dirfrac.y;
		float t4 = (b.boxMax.y - ray.origin.y) * dirfrac.y;
		float t5 = (b.boxMin.z - ray.origin.z) * dirfrac.z;
		float t6 = (b.boxMax.z - ray.origin.z) * dirfrac.z;

		float tmin = max(max(min(t1, t2), min(t3, t4)), min(t5, t6));
		float tmax = min(min(max(t1, t2), max(t3, t4)), max(t5, t6));

		if (tmax >= 0 && tmin < tmax)
		{
			float t = tmin;

			if (t < minTBox)
			{
				minTBox = t;
				boxIndex = i;
			}
		}
	}

	if (sphereIndex == -1 && boxIndex == -1)
	{
		return miss(ray);
	}

	if (minTSphere < minTBox)
	{
		return closestHitSphere(ray, minTSphere, sphereIndex);
	}
	else
	{
		return closestHitBox(ray, minTBox, boxIndex);
	}
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

	for (int i = 0; i < MAX_BOUNCES; i++)
	{
		RayPayload payload = traceRay(Ray(startPos, direction));

		seed += i;
		seed = pcg_hash(seed);

		if (payload.materialIndex < 0)
		{
			break;
		}
		else
		{
			Material mat = MATERIALS[payload.materialIndex];

			contribution *= mat.albedo;
			light += mat.albedo * mat.emission;

			seed += payload.materialIndex;
			seed = pcg_hash(seed);

			startPos = payload.position + payload.normal * EPSILON;
			direction = reflect(direction, payload.normal + mat.roughness * randomOnHemisphere(seed, payload.normal));
		}
	}

	return light * contribution;
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