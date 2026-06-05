#pragma once

class IGUIWindow
{
public:
	virtual ~IGUIWindow() = default;
	virtual void Draw() = 0;
};