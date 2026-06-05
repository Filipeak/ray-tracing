#pragma once

struct WindowConfig
{
	bool vsync;
};

struct CameraProjectionSettings
{
	float fov;
	float nearPlane;
	float farPlane;
};

struct RayTracingConfig
{
	bool accumulate;
};

struct BloomConfig
{
	bool enabled;
	int mipChainLength;
	float filterRadius;
	float strength;
};

struct ToneMapConfig
{
	float exposure;
	float gamma;
};

struct BVHDebugConfig
{
	bool enabled;
};