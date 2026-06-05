#include "Time.h"

void Time::Update()
{
	const auto now = Clock::now();
	m_Delta = std::chrono::duration<float>(now - m_Last).count();
	m_Total = std::chrono::duration<float>(now - m_Start).count();
	m_Last = now;
	m_FrameCount++;
}