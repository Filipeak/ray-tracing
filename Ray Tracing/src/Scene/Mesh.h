#pragma once

#include <vector>
#include <glm/glm.hpp>
#include "Material.h"

struct Mesh
{
	std::vector<glm::vec3> vertices;
	std::vector<unsigned int> indices;
	std::vector<Material> materials;
	std::vector<unsigned int> materialIds;

	void Merge(const Mesh& other);
};