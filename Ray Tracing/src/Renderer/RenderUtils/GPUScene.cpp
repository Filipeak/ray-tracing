#include "GPUScene.h"

struct OptimizedBVHTriangleInfo
{
	glm::vec4 v0; // .w is material index
	glm::vec4 v1;
	glm::vec4 v2;
};

static_assert(sizeof(OptimizedBVHTriangleInfo) == 48);


struct OptimizedBVHNode
{
	glm::vec3 minBounds;
	GLuint leftFirst;
	glm::vec3 maxBounds;
	GLuint triCount;
};

static_assert(sizeof(OptimizedBVHNode) == 32);


struct OptimizedMaterial
{
	glm::vec4 albedo; // RGB = albedo color, A = rougness
	glm::vec4 emission; // RGB = emission color, A = emission strength
	glm::vec4 params; // R = metallic, G = unused, B = unused, A = unused
};

static_assert(sizeof(OptimizedMaterial) == 48);


GPUScene::GPUScene(Scene& scene) : m_Scene(scene)
{
	Rebuild();
}

void GPUScene::Rebuild()
{
	Mesh mesh = m_Scene.BuildCombinedMesh();

	m_BVHBuilder.Build(mesh, 4, 32);

	BuildOptimizedNodes();
	BuildOptimizedTriangles();
	BuildOptimizedMaterials(mesh);

	m_RebuildCounter++;
}

void GPUScene::Bind() const
{
	m_NodesBuffer->Bind(0);
	m_TrianglesBuffer->Bind(1);
	m_MaterialsBuffer->Bind(2);
}

void GPUScene::Unbind() const
{
	m_NodesBuffer->Unbind();
	m_TrianglesBuffer->Unbind();
	m_MaterialsBuffer->Unbind();
}

void GPUScene::BuildOptimizedNodes()
{
	const auto& nodes = m_BVHBuilder.GetNodes();
	size_t nodesUsed = m_BVHBuilder.GetNodesUsed();

	std::vector<OptimizedBVHNode> optimizedNodes;
	optimizedNodes.reserve(nodesUsed);

	for (size_t i = 0; i < nodesUsed; i++)
	{
		OptimizedBVHNode node{};
		node.minBounds = nodes[i].bounds.minBounds;
		node.leftFirst = (GLuint)nodes[i].leftFirst;
		node.maxBounds = nodes[i].bounds.maxBounds;
		node.triCount = (GLuint)nodes[i].triCount;

		optimizedNodes.push_back(node);
	}

	m_NodesBuffer = std::make_unique<ShaderStorageBuffer>(optimizedNodes.data(), optimizedNodes.size(), sizeof(optimizedNodes[0]));
}

void GPUScene::BuildOptimizedTriangles()
{
	const auto& triangleInfos = m_BVHBuilder.GetTriangleInfos();

	std::vector<OptimizedBVHTriangleInfo> optimizedTriangleInfos;
	optimizedTriangleInfos.reserve(triangleInfos.size());

	for (size_t i = 0; i < triangleInfos.size(); i++)
	{
		OptimizedBVHTriangleInfo info{};
		info.v0 = glm::vec4(triangleInfos[i].v0, (float)triangleInfos[i].materialIndex);
		info.v1 = glm::vec4(triangleInfos[i].v1, 0.0f);
		info.v2 = glm::vec4(triangleInfos[i].v2, 0.0f);

		optimizedTriangleInfos.push_back(info);
	}

	m_TrianglesBuffer = std::make_unique<ShaderStorageBuffer>(optimizedTriangleInfos.data(), optimizedTriangleInfos.size(), sizeof(optimizedTriangleInfos[0]));
}

void GPUScene::BuildOptimizedMaterials(const Mesh& mesh)
{
	std::vector<OptimizedMaterial> optimizedMaterials(mesh.materials.size());

	for (size_t i = 0; i < optimizedMaterials.size(); i++)
	{
		optimizedMaterials[i] = {
			glm::vec4(mesh.materials[i].albedo, mesh.materials[i].roughness),
			glm::vec4(mesh.materials[i].emissionColor, mesh.materials[i].emissionStrength),
			glm::vec4(mesh.materials[i].metallic, 0.0f, 0.0f, 0.0f)
		};
	}

	m_MaterialsBuffer = std::make_unique<ShaderStorageBuffer>(optimizedMaterials.data(), optimizedMaterials.size(), sizeof(optimizedMaterials[0]));
}