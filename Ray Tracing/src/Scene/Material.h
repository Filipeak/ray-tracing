#pragma once

#include <glm/glm.hpp>

struct Material
{
	glm::vec3 albedo;
	glm::vec3 emissionColor;
	float emissionStrength = 0.0f;
	float roughness = 0.0f;
	float metallic = 0.0f;
};