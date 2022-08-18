#include "Input.h"

#include "Core/SMemory.h"
#include "Core/Logger.h"

namespace Scal
{
namespace Input
{

struct KeyboardState
{
	bool Keys[(int)Keys::MaxKeys];
};

struct MouseState
{
	MousePos Pos;
	int MouseDelta;
	bool Buttons[(int)Buttons::MaxButtons];
};

struct InputState
{
	KeyboardState KeyboardCur;
	KeyboardState KeyboardPrev;
	MouseState MouseCur;
	MouseState MousePrev;
};


global_var bool Initialized;
global_var InputState State;

void InputInitialize()
{


	Initialized = true;
	SINFO("Successfully initialized Input!");
}

void InputShutdown()
{
	Initialized = false;
}


void InputUpdate(float dt)
{
	if (!Initialized) return;

	State.KeyboardPrev = State.KeyboardCur;
	State.MousePrev = State.MouseCur;
}

void ProcessKey(Keys key, bool pressed)
{
	State.KeyboardCur.Keys[(int)key] = pressed;
}

void ProcessMouse(Buttons button, bool pressed)
{
	State.MouseCur.Buttons[(int)button] = pressed;
}


void ProcessMouseMove(int x, int y)
{
	State.MouseCur.Pos.X = x;
	State.MouseCur.Pos.Y = y;
}

void ProcessMouseWheel(int delta)
{
	State.MouseCur.MouseDelta = delta;
}

SAPI bool IsKeyDown(Keys key)
{
	if (!Initialized) return false;
	return State.KeyboardCur.Keys[(int)key];
}

SAPI bool IsKeyUp(Keys key)
{
	if (!Initialized) return true;
	return !State.KeyboardCur.Keys[(int)key];
}

SAPI bool WasKeyDown(Keys key)
{
	if (!Initialized) return false;
	return State.KeyboardPrev.Keys[(int)key];
}

SAPI bool WasKeyUp(Keys key)
{
	if (!Initialized) return true;
	return !State.KeyboardPrev.Keys[(int)key];
}

SAPI bool IsMouseDown(Buttons key)
{
	if (!Initialized) return false;
	return State.MouseCur.Buttons[(int)key];
}

SAPI bool IsMouseUp(Buttons key)
{
	if (!Initialized) return true;
	return !State.MouseCur.Buttons[(int)key];
}

SAPI bool WasMouseDown(Buttons key)
{
	if (!Initialized) return false;
	return State.MousePrev.Buttons[(int)key];
}

SAPI bool WasMouseUp(Buttons key)
{
	if (!Initialized) return true;
	return !State.MousePrev.Buttons[(int)key];
}

SAPI int GetMouseWheelDelta()
{
	if (!Initialized) return false;
	return State.MouseCur.MouseDelta;
}

SAPI MousePos GetMousePosition()
{
	if (!Initialized) return { 0, 0 };
	return State.MouseCur.Pos;
}

SAPI MousePos GetPrevMousePosition()
{
	if (!Initialized) return { 0, 0 };
	return State.MousePrev.Pos;
}

}
}
