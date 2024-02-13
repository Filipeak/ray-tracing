#pragma once

#include "Window.h"
#include <glm/glm.hpp>

class Camera
{
public:
	Camera();

	void Update(const Window& window);

	const glm::vec3& GetPos() const;

private:
	glm::vec3 m_Position;
	glm::vec3 m_Rotation;
};