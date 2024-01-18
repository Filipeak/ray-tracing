#version 330 core


layout(location = 0) out vec4 FragColor;

uniform vec2 u_Resolution;


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
	float roughness;
	float metallic;
	vec3 emissionColor;
	float emissionPower;
};

struct SphereData
{
	vec3 origin;
	float radius;
	int materialIndex;
};


#define BOUNCES 5


const Material MATERIALS[] = Material[](
	Material(vec3(1, 0, 1), 1, 0, vec3(0, 0, 0), 0),
	Material(vec3(0, 1, 1), 1, 0, vec3(0, 0, 0), 0)
);

const SphereData SPHERES[] = SphereData[](
	SphereData(vec3(0, 0, 0), 1.0f, 0),
	SphereData(vec3(0, -101.0f, 0), 100.0f, 1)
);


float rand(float minf, float maxf){
    return fract(sin(dot(vec2(minf, maxf), vec2(12.9898, 78.233))) * 43758.5453);
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

	vec3 origin = ray.origin - SPHERES[sphereIndex].origin;

	payload.position = origin + ray.dir * minT;
	payload.normal = normalize(payload.position);

	payload.position += SPHERES[sphereIndex].origin;

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
		float b = 2 * (origin.x * ray.dir.x + origin.y * ray.dir.y + origin.z * ray.dir.z);
		float c = origin.x * origin.x + origin.y * origin.y + origin.z * origin.z - SPHERES[i].radius * SPHERES[i].radius;

		float discriminant = b * b - 4 * a * c;

		if (discriminant < 0)
		{
			continue;
		}

		float t = (-b - sqrt(discriminant)) / (2 * a);

		if (t > 0 && t < minT)
		{
			minT = t;
			sphereIndex = i;
		}
	}

	if (sphereIndex == -1)
	{
		return miss(ray);
	}

	return closestHit(ray, minT, sphereIndex);
}

vec4 rayGen(vec3 startPos, vec3 direction)
{
	vec3 light = vec3(0, 0, 0);
	vec3 contribution = vec3(1, 1, 1);

	for (int i = 0; i < BOUNCES; i++)
	{
		RayPayload payload = traceRay(Ray(startPos, direction));

		if (payload.sphereIndex < 0)
		{
			vec3 skyColor = vec3(0.5, 0.7, 0.9);

			light += skyColor * contribution;

			break;
		}

		Material material = MATERIALS[SPHERES[payload.sphereIndex].materialIndex];

		light += material.emissionColor * material.emissionPower;
		contribution *= material.albedo;

		startPos = payload.position + payload.normal * 0.0001f;
		direction = reflect(direction, payload.normal + material.roughness * vec3(rand(-0.5f, 0.5f), rand(-0.5f, 0.5f), rand(-0.5f, 0.5f)));
	}

	return vec4(light, 1);
}


void main()
{
	float aspectRatio = u_Resolution.x / u_Resolution.y;
	vec2 uv = gl_FragCoord.xy / u_Resolution.xy * 2.0f - 1;
	uv.x *= aspectRatio;

	FragColor = rayGen(vec3(0, 0, -2), vec3(uv, 1));
}