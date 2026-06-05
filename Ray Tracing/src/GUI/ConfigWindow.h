#pragma once

#include "IGUIWindow.h"
#include "Core/Config.h"
#include "Scene/Camera/Camera.h"
#include "Scene/Camera/FlyCameraController.h"

class ConfigWindow : public IGUIWindow
{
public:
	ConfigWindow(WindowConfig& windowConfig, RayTracingConfig& rayTraceConfig, BloomConfig& bloomConfig, ToneMapConfig& toneMapConfig, FlyCameraSettings& cameraSettings, CameraProjectionSettings& projectionSettings)
		: m_WindowConfig(windowConfig), m_RayTracingConfig(rayTraceConfig), m_BloomConfig(bloomConfig), m_ToneMapConfig(toneMapConfig), m_CameraSettings(cameraSettings), m_ProjectionSettings(projectionSettings)
	{
	}

	void Draw();

private:
	WindowConfig& m_WindowConfig;
	RayTracingConfig& m_RayTracingConfig;
	BloomConfig& m_BloomConfig;
	ToneMapConfig& m_ToneMapConfig;
	FlyCameraSettings& m_CameraSettings;
	CameraProjectionSettings& m_ProjectionSettings;
};