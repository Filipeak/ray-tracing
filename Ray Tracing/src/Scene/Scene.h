#pragma once

#include <memory>
#include "Mesh.h"
#include "Camera/Camera.h"
#include "Camera/ICameraController.h"

class Scene
{
public:
	void AddMesh(const Mesh& mesh);
	Mesh BuildCombinedMesh();

	void SetCamera(std::unique_ptr<Camera> camera) { m_Camera = std::move(camera); }
	void SetCameraController(std::unique_ptr<ICameraController> controller) { m_CameraController = std::move(controller); }
	Camera& GetCamera() { return *m_Camera; }

	void Update(float dt);

private:
	std::vector<Mesh> m_Meshes;
	std::unique_ptr<Camera> m_Camera;
	std::unique_ptr<ICameraController> m_CameraController;
};