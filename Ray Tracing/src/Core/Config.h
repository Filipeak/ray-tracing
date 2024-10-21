#pragma once

struct ConfigData
{
	bool vsync = false;
	bool accumulate = true;
	float hdrExposure = 1.0f;
	float screenGamma = 2.2f;
	bool bloom = true;
	int bloomMipChains = 5;
	float bloomStrength = 0.05f;
	float bloomFilterRadius = 0.005f;
	float cameraSpeed = 7.0f;
	float cameraSens = 3.5f;
	float cameraFOV = 70.0f;
};