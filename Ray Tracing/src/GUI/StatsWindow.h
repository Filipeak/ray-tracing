#pragma once

#include "IGUIWindow.h"
#include "Platform/Time.h"

class StatsWindow : public IGUIWindow
{
public:
	StatsWindow(const Time& time) : m_Time(time) {}

	void Draw();

private:
	const Time& m_Time;
};