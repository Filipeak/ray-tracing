#pragma once

#include "Camera.h"

class ICameraController
{
public:
	virtual ~ICameraController() = default;
	virtual void Update(Camera& camera, float dt) = 0;
};