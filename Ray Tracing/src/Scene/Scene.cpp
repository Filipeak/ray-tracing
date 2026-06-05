#include "Scene.h"
#include "Core/Logger.h"

void Scene::AddMesh(const Mesh& mesh)
{
	m_Meshes.push_back(mesh);
}

Mesh Scene::BuildCombinedMesh()
{
	if (m_Meshes.empty())
	{
		return Mesh();
	}

	Mesh combinedMesh = m_Meshes[0];

	for (size_t i = 1; i < m_Meshes.size(); ++i)
	{
		combinedMesh.Merge(m_Meshes[i]);
	}

	spdlog::debug("Combined mesh has {} vertices, {} indices, {} materials, and {} material IDs", combinedMesh.vertices.size(), combinedMesh.indices.size(), combinedMesh.materials.size(), combinedMesh.materialIds.size());

	return combinedMesh;
}

void Scene::Update(float dt)
{
	m_CameraController->Update(GetCamera(), dt);
}