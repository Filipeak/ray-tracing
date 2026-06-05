#include "BVHBuilder.h"
#include <algorithm>

void BVHBuilder::Build(const Mesh& mesh, size_t maxTrianglesPerLeaf, int maxDepth)
{
	SetupTrianglesInfo(mesh);
	SetupNodes();

	BVHNode& rootNode = m_Nodes[0];
	rootNode.leftFirst = 0;
	rootNode.triCount = (int)m_TriangleInfos.size();
	m_NodesUsed++;

	UpdateNodeBounds(rootNode);
	SubdivideNode(rootNode, maxTrianglesPerLeaf, maxDepth);
}

void BVHBuilder::SetupTrianglesInfo(const Mesh& mesh)
{
	size_t triangleCount = mesh.indices.size() / 3;

	m_TriangleInfos.clear();
	m_TriangleInfos.reserve(triangleCount);

	for (size_t i = 0; i < triangleCount; i++)
	{
		size_t triangleOffset = i * 3;
		glm::vec3 v0 = mesh.vertices[mesh.indices[triangleOffset + 0]];
		glm::vec3 v1 = mesh.vertices[mesh.indices[triangleOffset + 1]];
		glm::vec3 v2 = mesh.vertices[mesh.indices[triangleOffset + 2]];

		m_TriangleInfos.push_back({ v0, v1, v2, (v0 + v1 + v2) / 3.0f, (int)mesh.materialIds[i] });
	}
}

void BVHBuilder::SetupNodes()
{
	m_Nodes.clear();
	m_Nodes.resize(m_TriangleInfos.size() * 2 - 1); // Worst case: each triangle becomes a leaf node
	m_NodesUsed = 0;
}

void BVHBuilder::UpdateNodeBounds(BVHNode& node)
{
	node.bounds = AABB{};

	for (int i = 0; i < node.triCount; i++)
	{
		const BVHTriangleInfo& tri = m_TriangleInfos[(size_t)node.leftFirst + i];

		node.bounds.Grow(tri.v0);
		node.bounds.Grow(tri.v1);
		node.bounds.Grow(tri.v2);
	}

	// Prevent degenerate (zero-extent) AABBs — flat boxes fail the slab ray test
	const float eps = 1e-4f;
	node.bounds.minBounds -= glm::vec3(eps);
	node.bounds.maxBounds += glm::vec3(eps);
}

struct Bin
{
	AABB bounds{};
	int count = 0;
};

void BVHBuilder::SubdivideNode(BVHNode& node, size_t maxTrianglesPerLeaf, int maxDepth)
{
	if (node.triCount <= maxTrianglesPerLeaf || maxDepth-- <= 0)
	{
		return;
	}

	AABB centroidBounds{};
	for (int i = 0; i < node.triCount; ++i)
	{
		centroidBounds.Grow(m_TriangleInfos[(size_t)node.leftFirst + i].centroid);
	}

	glm::vec3 extent = centroidBounds.Extent();

	const int BIN_COUNT = 16;
	float bestCost = 1e30f;
	int bestAxis = -1;
	float bestSplitPosition = 0.0f;

	for (int axis = 0; axis < 3; axis++)
	{
		if (extent[axis] <= 1e-6f)
		{
			continue; // Can't split along this axis if all centroids are the same
		}

		// Compute bins
		Bin bins[BIN_COUNT]{};
		float scale = BIN_COUNT / extent[axis];

		for (int i = 0; i < node.triCount; ++i)
		{
			float c = m_TriangleInfos[(size_t)node.leftFirst + i].centroid[axis];
			int binIndex = std::clamp((int)((c - centroidBounds.minBounds[axis]) * scale), 0, BIN_COUNT - 1);
			bins[binIndex].count++;
			bins[binIndex].bounds.Grow(m_TriangleInfos[(size_t)node.leftFirst + i].v0);
			bins[binIndex].bounds.Grow(m_TriangleInfos[(size_t)node.leftFirst + i].v1);
			bins[binIndex].bounds.Grow(m_TriangleInfos[(size_t)node.leftFirst + i].v2);
		}

		// Compute prefix sums for left side and suffix sums for right side to evaluate SAH cost of splits
		float leftArea[BIN_COUNT - 1]{};
		int leftCount[BIN_COUNT - 1]{};
		AABB leftRunningBounds{};
		int leftRunningCount = 0;

		for (int i = 0; i < BIN_COUNT - 1; i++)
		{
			if (bins[i].count > 0)
			{
				leftRunningBounds.Grow(bins[i].bounds.minBounds);
				leftRunningBounds.Grow(bins[i].bounds.maxBounds);
			}

			leftRunningCount += bins[i].count;
			leftArea[i] = leftRunningBounds.SurfaceArea();
			leftCount[i] = leftRunningCount;
		}

		float rightArea[BIN_COUNT - 1]{};
		int rightCount[BIN_COUNT - 1]{};
		AABB rightRunningBox{};
		int rightRunningSum = 0;

		for (int i = BIN_COUNT - 1; i > 0; i--)
		{
			if (bins[i].count > 0)
			{
				rightRunningBox.Grow(bins[i].bounds.minBounds);
				rightRunningBox.Grow(bins[i].bounds.maxBounds);
			}

			rightRunningSum += bins[i].count;
			rightArea[i - 1] = rightRunningBox.SurfaceArea();
			rightCount[i - 1] = rightRunningSum;
		}

		// Evaluate splits and find the best one
		for (int i = 0; i < BIN_COUNT - 1; i++)
		{
			// Calculate SAH Cost
			// Cost = (Area_Left * Count_Left) + (Area_Right * Count_Right)
			float splitCost = leftArea[i] * leftCount[i] + rightArea[i] * rightCount[i];

			if (splitCost < bestCost && leftCount[i] > 0 && rightCount[i] > 0)
			{
				bestCost = splitCost;
				bestAxis = axis;
				bestSplitPosition = centroidBounds.minBounds[axis] + (i + 1) * (extent[axis] / BIN_COUNT);
			}
		}
	}

	float leafCost = node.triCount * node.bounds.SurfaceArea();

	if (bestCost >= leafCost || bestAxis == -1)
	{
		return; // It is cheaper to make a leaf here than to split!
	}

	auto splitIter = std::partition(m_TriangleInfos.begin() + node.leftFirst, m_TriangleInfos.begin() + node.leftFirst + node.triCount, [bestAxis, bestSplitPosition](const auto& info) { return info.centroid[bestAxis] < bestSplitPosition; });
	int leftCount = (int)std::distance(m_TriangleInfos.begin() + node.leftFirst, splitIter);

	if (leftCount == 0 || leftCount == node.triCount)
	{
		// Object Median fallback: split in the middle if SAH failed to find a good split (can happen with many overlapping triangles)
		int midIndex = node.leftFirst + node.triCount / 2;
		std::nth_element(m_TriangleInfos.begin() + node.leftFirst, m_TriangleInfos.begin() + midIndex, m_TriangleInfos.begin() + node.leftFirst + node.triCount, [bestAxis](const auto& a, const auto& b) { return a.centroid[bestAxis] < b.centroid[bestAxis]; });
		leftCount = node.triCount / 2;
	}

	unsigned int leftChildIndex = (unsigned int)m_NodesUsed++;
	unsigned int rightChildIndex = (unsigned int)m_NodesUsed++;

	m_Nodes[leftChildIndex].leftFirst = node.leftFirst;
	m_Nodes[leftChildIndex].triCount = leftCount;
	m_Nodes[rightChildIndex].leftFirst = node.leftFirst + leftCount;
	m_Nodes[rightChildIndex].triCount = node.triCount - leftCount;

	node.leftFirst = leftChildIndex;
	node.triCount = 0;

	UpdateNodeBounds(m_Nodes[leftChildIndex]);
	UpdateNodeBounds(m_Nodes[rightChildIndex]);

	SubdivideNode(m_Nodes[leftChildIndex], maxTrianglesPerLeaf, maxDepth);
	SubdivideNode(m_Nodes[rightChildIndex], maxTrianglesPerLeaf, maxDepth);
}