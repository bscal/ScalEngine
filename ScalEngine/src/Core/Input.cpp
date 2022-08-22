#include "Input.h"

#include "Core/SMemory.h"
#include "Core/Logger.h"

namespace Scal
{
namespace Input
{

struct KeyState
{
	uint8_t State;

	bool IsDown() const
	{
		return State >> 0 & 1;
	}

	bool IsPressed() const
	{
		return State >> 1 & 1;
	}

	bool IsHeld() const
	{
		return State >> 2 & 1;
	}

	bool IsReleased() const
	{
		return State >> 3 & 1;
	}
};

struct KeyboardState
{
	KeyState Keys[(int)Keys::MaxKeys];
};

struct MouseState
{
	MousePos Pos;
	int MouseDelta; // Value between -1 and 1; 0 is Unchanged; 
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

int InitializeInput()
{
	Initialized = true;
	SINFO("Successfully initialized Input!");
	return 0;
}

void ShutdownInput()
{
	Initialized = false;
}


void UpdateInput()
{
	State.KeyboardPrev = State.KeyboardCur;
	State.MousePrev = State.MouseCur;
	SZero(&State.KeyboardCur, sizeof(KeyboardState));
	SZero(&State.MouseCur, sizeof(MouseState));
}

void ProcessKey(Keys key, bool isDown, bool pressed, bool held, bool released)
{
	uint8_t keyState = 0;
	keyState |= isDown << 0;
	keyState |= pressed << 1;
	keyState |= held << 2;
	keyState |= released << 3;
	State.KeyboardCur.Keys[(int)key].State = keyState;
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
	return State.KeyboardCur.Keys[(int)key].IsDown();
}

SAPI bool IsKeyUp(Keys key)
{
	if (!Initialized) return true;
	return !State.KeyboardCur.Keys[(int)key].IsDown();
}

SAPI bool WasKeyDown(Keys key)
{
	if (!Initialized) return false;
	return State.KeyboardPrev.Keys[(int)key].IsDown();
}

SAPI bool WasKeyUp(Keys key)
{
	if (!Initialized) return true;
	return !State.KeyboardPrev.Keys[(int)key].IsDown();
}

SAPI bool IsKeyPressed(Keys key)
{
	if (!Initialized) return false;
	return State.KeyboardCur.Keys[(int)key].IsPressed();
}

SAPI bool IsKeyHeld(Keys key)
{
	if (!Initialized) return false;
	return State.KeyboardCur.Keys[(int)key].IsHeld();
}

SAPI bool IsKeyReleased(Keys key)
{
	if (!Initialized) return false;
	return State.KeyboardCur.Keys[(int)key].IsReleased();
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
