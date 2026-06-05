#pragma once

#include "Platform/Input.h"
#include "ICameraController.h"
#include <glm/glm.hpp>

struct FlyCameraSettings
{
	float speed;
	float sensitivity;
};

class FlyCameraController : public ICameraController
{
public:
	FlyCameraController(Input& input, const FlyCameraSettings& settings);

	void Update(Camera& camera, float dt) override;

private:
	Input& m_Input;
	const FlyCameraSettings& m_Settings;
};