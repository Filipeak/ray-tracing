#pragma once

#include <memory>
#include "Scene.h"
#include "Camera/Camera.h"
#include "Camera/ICameraController.h"

class CornellBox
{
public:
	CornellBox(std::unique_ptr<ICameraController> cameraController);

	Scene& GetScene() { return m_Scene; }

private:
	Scene m_Scene;
};