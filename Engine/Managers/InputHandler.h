#pragma once

struct Keys
{
	bool Left = false;
	bool Right = false;
};

struct Mouse
{
	bool Left = false;
	float x = 0;
	float y = 0;
};

class InputHandler
{
public:
	InputHandler(const InputHandler&) = delete;
	InputHandler& operator=(const InputHandler&) = delete;

	static InputHandler& GetInstance()
	{
		static InputHandler instance;
		return instance;
	}

	Keys m_keys;
	Mouse m_mouse;

private:
	InputHandler() {}
};