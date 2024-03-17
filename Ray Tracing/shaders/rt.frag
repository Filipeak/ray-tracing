#version 430 core


#define BOUNCES 5
#define SAMPLES 1
#define EPSILON 0.0001f
#define LIGHT_AMBIENT_FACTOR 0.3f
#define LIGHT_DIFFUSE_FACTOR 1.0f
#define LIGHT_SPECULAR_FACTOR 0.6f


struct Ray
{
	vec3 origin;
	vec3 dir;
};

struct RayPayload
{
	int sphereIndex;
	vec3 position;
	vec3 normal;
};

struct Material
{
	vec3 albedo;
	vec3 specular;
	float shininess;
	float roughness;
};

struct Sphere
{
	vec3 origin;
	float radius;
	int materialIndex;
};

struct DirectionalLight
{
	vec3 direction;
	vec3 color;
	float power;
};


layout(location = 0) out vec4 FragColor;


uniform vec2 u_Resolution;
uniform float u_Time;
uniform vec3 u_CameraPosition;
uniform mat4x4 u_CameraInverseProjection;
uniform mat4x4 u_CameraInverseView;


float randomOffset;


const Material MATERIALS[] = Material[]
(
	Material(vec3(1, 0, 1), vec3(1, 1, 1), 64.0f, 0.1f),
	Material(vec3(1, 0.5, 0), vec3(1, 1, 1), 32.0f, 0.7f)
);

const Sphere SPHERES[] = Sphere[]
(
	Sphere(vec3(0, 0, 0), 1.0f, 0),
	Sphere(vec3(0, -101.0f, 0), 100.0f, 1)
);

const DirectionalLight DIRECTIONAL_LIGHTS[] = DirectionalLight[]
(
	DirectionalLight(normalize(vec3(0, -1, 1)), vec3(1, 1, 1), 1.0f)
);

const vec3 SKYBOX_COLOR = vec3(0.5f, 0.7f, 0.9f);


uint hash(uint x)
{
    x += (x << 10u);
    x ^= (x >> 6u);
    x += (x << 3u);
    x ^= (x >> 11u);
    x += (x << 15u);

    return x;
}

float random01()
{
	uvec4 v = floatBitsToUint(vec4(gl_FragCoord.xy, u_Time, randomOffset));
	uint m = hash(v.x ^ hash(v.y) ^ hash(v.z) ^ hash(v.w)); 

    const uint ieeeMantissa = 0x007FFFFFu;
    const uint ieeeOne = 0x3F800000u;

    m &= ieeeMantissa;
    m |= ieeeOne;

    float f = uintBitsToFloat(m);

	randomOffset++;

    return f - 1.0;
}

float random(float minf, float maxf)
{
	return random01() * (maxf - minf) + minf;
}

vec3 randomVec3(float minf, float maxf)
{
	return vec3(random(minf, maxf), random(minf, maxf), random(minf, maxf));
}

vec3 randomOnUnitSphere()
{
	return normalize(randomVec3(-1, 1));
}

vec3 randomOnHemisphere(vec3 normal)
{
	vec3 onUnitSphere = randomOnUnitSphere();

	return sign(dot(onUnitSphere, normal)) * onUnitSphere;
}


vec3 calculateIlluminationForSingleLight(Material material, vec3 lightColor, float lightPower, vec3 lightDir, vec3 normal)
{
	vec3 ambient = lightColor * material.albedo;
	vec3 diffuse = vec3(0, 0, 0);
	vec3 specular = vec3(0, 0, 0);

	float diff = dot(normal, lightDir);

	if (diff > 0)
	{
		diffuse = lightColor * lightPower * diff * material.albedo;

		vec3 halfwayDir = normalize(lightDir + normal);
		float specFactor = dot(normal, halfwayDir);

		if (specFactor > 0)
		{
			float spec = pow(specFactor, material.shininess);

			specular = lightColor * lightPower * spec * material.specular;
		}
	}

	return LIGHT_AMBIENT_FACTOR * ambient + LIGHT_DIFFUSE_FACTOR * diffuse + LIGHT_SPECULAR_FACTOR * specular;
}

vec3 calculateIllumination(Material material, vec3 position, vec3 normal, vec3 viewDir)
{
	vec3 result = vec3(0, 0, 0);

	for (int i = 0; i < DIRECTIONAL_LIGHTS.length(); i++)
	{
		result += calculateIlluminationForSingleLight(
			material,
			DIRECTIONAL_LIGHTS[i].color,
			DIRECTIONAL_LIGHTS[i].power,
			-DIRECTIONAL_LIGHTS[i].direction,
			normal
		);
	}

	return result;
}


RayPayload miss(Ray ray)
{
	RayPayload payload;
	payload.sphereIndex = -1;

	return payload;
}

RayPayload closestHit(Ray ray, float minT, int sphereIndex)
{
	RayPayload payload;
	payload.sphereIndex = sphereIndex;
	payload.position = ray.origin + ray.dir * minT;
	payload.normal = (payload.position - SPHERES[sphereIndex].origin) / SPHERES[sphereIndex].radius;

	if (dot(ray.dir, payload.normal) > 0)
	{
		payload.normal = -payload.normal;
	}

	return payload;
}

RayPayload traceRay(Ray ray)
{
	float minT = 99999999;
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
			if (t < minT)
			{
				minT = t;
				sphereIndex = i;
			}
		}
		else
		{
			t = (-half_b + sqrt(discriminant)) / a;

			if (t > 0)
			{
				if (t < minT)
				{
					minT = t;
					sphereIndex = i;
				}
			}
		}
	}

	if (sphereIndex == -1)
	{
		return miss(ray);
	}

	return closestHit(ray, minT, sphereIndex);
}

vec3 rayGen(vec2 rayOffset)
{
	vec2 coord = (gl_FragCoord.xy + rayOffset) / u_Resolution.xy * 2.0f - 1;
	vec4 target = u_CameraInverseProjection * vec4(coord.x, coord.y, 1, 1);
	vec3 viewDir = vec3(u_CameraInverseView * vec4(normalize(vec3(target) / target.w), 0));
	vec3 startPos = u_CameraPosition;
	vec3 direction = viewDir;

	vec3 color = vec3(0, 0, 0);
	float multiplier = 1.0f;

	for (int i = 0; i < BOUNCES; i++)
	{
		RayPayload payload = traceRay(Ray(startPos, direction));

		if (payload.sphereIndex < 0)
		{
			color += SKYBOX_COLOR * multiplier;

			break;
		}
		else
		{
			Material mat = MATERIALS[SPHERES[payload.sphereIndex].materialIndex];
			vec3 illuminationColor = calculateIllumination(mat, payload.position, payload.normal, viewDir);

			multiplier *= 0.5f;
			color += illuminationColor * multiplier;

			startPos = payload.position + payload.normal * EPSILON;
			direction = reflect(direction, payload.normal) + mat.roughness * randomOnHemisphere(payload.normal);
		}
	}

	return color;
}


vec3 samplePixel()
{
	vec3 result = vec3(0, 0, 0);

	for (int i = 0; i < SAMPLES; i++)
	{
		result += rayGen(vec2(random(-0.5f, 0.5f), random(-0.5f, 0.5f)));
	}

	return result / SAMPLES;
}


void main()
{
	FragColor = vec4(samplePixel(), 1.0f);
}