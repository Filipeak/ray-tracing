#pragma once

#include "Scene/Camera/Camera.h"
#include "Renderer/RenderUtils/GPUScene.h"

struct RenderViewport
{
	int width;
	int height;
};

struct RenderContext
{
	const Camera& camera;
	const RenderViewport& viewport;
	const GPUScene& gpuScene;
};