#pragma once

#include <chrono>
#include <cstdint>

class Time
{
public:
	void Update();

	float GetDeltaSeconds() const { return m_Delta; }
	float GetTotalSeconds() const { return m_Total; }
	uint32_t GetFrameCount() const { return m_FrameCount; }

private:
	using Clock = std::chrono::steady_clock;

	Clock::time_point m_Start = Clock::now();
	Clock::time_point m_Last = Clock::now();
	float m_Delta = 0.0f;
	float m_Total = 0.0f;
	uint32_t m_FrameCount = 0;
};