#include "Mesh.h"

void Mesh::Merge(const Mesh& other)
{
	size_t vertexOffset = vertices.size();
	size_t indexOffset = indices.size();
	size_t materialOffset = materials.size();
	size_t materialIdsOffset = materialIds.size();

	vertices.insert(vertices.end(), other.vertices.begin(), other.vertices.end());
	indices.insert(indices.end(), other.indices.begin(), other.indices.end());
	materials.insert(materials.end(), other.materials.begin(), other.materials.end());
	materialIds.insert(materialIds.end(), other.materialIds.begin(), other.materialIds.end());

	for (size_t i = indexOffset; i < indices.size(); i++)
	{
		indices[i] += (unsigned int)vertexOffset;
	}

	for (size_t i = materialIdsOffset; i < materialIds.size(); i++)
	{
		materialIds[i] += (unsigned int)materialOffset;
	}
}