#pragma once

#include "Scene/Scene.h"
#include "BVHBuilder.h"
#include "Renderer/Abstractions/ShaderStorageBuffer.h"

class GPUScene
{
public:
	GPUScene(Scene& scene);

	void Rebuild();

	void Bind() const;
	void Unbind() const;

	const BVHBuilder& GetBVHBuilder() const { return m_BVHBuilder; }
	int GetRebuildCounter() const { return m_RebuildCounter; }

private:
	Scene& m_Scene;
	BVHBuilder m_BVHBuilder;
	int m_RebuildCounter = 0;

	std::unique_ptr<ShaderStorageBuffer> m_NodesBuffer;
	std::unique_ptr<ShaderStorageBuffer> m_TrianglesBuffer;
	std::unique_ptr<ShaderStorageBuffer> m_MaterialsBuffer;

	void BuildOptimizedNodes();
	void BuildOptimizedTriangles();
	void BuildOptimizedMaterials(const Mesh& mesh);
};