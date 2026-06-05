#pragma once

#include "Scene/Mesh.h"
#include <glad/glad.h>
#include <vector>

struct AABB
{
	glm::vec3 minBounds = glm::vec3(std::numeric_limits<float>::max());
	glm::vec3 maxBounds = glm::vec3(std::numeric_limits<float>::lowest());

	void Grow(const glm::vec3& point)
	{
		minBounds = glm::min(minBounds, point);
		maxBounds = glm::max(maxBounds, point);
	}

	float SurfaceArea() const
	{
		glm::vec3 extent = Extent();
		return 2.0f * (extent.x * extent.y + extent.y * extent.z + extent.z * extent.x);
	}

	glm::vec3 Extent() const
	{
		return maxBounds - minBounds;
	}
};

struct BVHTriangleInfo
{
	glm::vec3 v0;
	glm::vec3 v1;
	glm::vec3 v2;
	glm::vec3 centroid;
	int materialIndex;
};

struct BVHNode
{
	AABB bounds;
	int leftFirst = 0; // For internal nodes: index of the left child; for leaf nodes: index of the first triangle in the triangle list
	int triCount = 0;
};

class BVHBuilder
{
public:
	void Build(const Mesh& mesh, size_t maxTrianglesPerLeaf, int maxDepth);

	const std::vector<BVHNode>& GetNodes() const { return m_Nodes; }
	const std::vector<BVHTriangleInfo>& GetTriangleInfos() const { return m_TriangleInfos; }
	const size_t GetNodesUsed() const { return m_NodesUsed; }

private:
	// Temporary variables used for BVH construction
	std::vector<BVHTriangleInfo> m_TriangleInfos;
	std::vector<BVHNode> m_Nodes;
	size_t m_NodesUsed = 0;

	// Setup functions
	void SetupTrianglesInfo(const Mesh& mesh);
	void SetupNodes();

	// Build functions
	void UpdateNodeBounds(BVHNode& node);
	void SubdivideNode(BVHNode& node, size_t maxTrianglesPerLeaf, int maxDepth);
};