#include "Input.h"

#include "Core/SMemory.h"
#include "Core/Logger.h"

namespace Scal
{
namespace Input
{

struct KeyState
{
	bool IsDown;
	bool IsPressed;
	bool IsHeld;
	bool IsReleased;
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
	KeyboardState KeyboardPrev; // TODO are previous really needed?
	MouseState MouseCur;
	MouseState MousePrev; // TODO are previous really needed?
	bool IsInitialized;
};

global_var InputState State;

int InitializeInput()
{
	State.IsInitialized = true;
	SINFO("Successfully initialized Input!");
	return 0;
}

void ShutdownInput()
{
	State.IsInitialized = false;
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
	State.KeyboardCur.Keys[(int)key].IsDown = isDown;
	State.KeyboardCur.Keys[(int)key].IsPressed = pressed;
	State.KeyboardCur.Keys[(int)key].IsHeld = held;
	State.KeyboardCur.Keys[(int)key].IsReleased = released;
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
	if (!State.IsInitialized) return false;
	return State.KeyboardCur.Keys[(int)key].IsDown;
}

SAPI bool WasKeyDown(Keys key)
{
	if (!State.IsInitialized) return false;
	return State.KeyboardPrev.Keys[(int)key].IsDown;
}

SAPI bool IsKeyPressed(Keys key)
{
	if (!State.IsInitialized) return false;
	return State.KeyboardCur.Keys[(int)key].IsPressed;
}

SAPI bool IsKeyHeld(Keys key)
{
	if (!State.IsInitialized) return false;
	return State.KeyboardCur.Keys[(int)key].IsHeld;
}

SAPI bool IsKeyReleased(Keys key)
{
	if (!State.IsInitialized) return false;
	return State.KeyboardCur.Keys[(int)key].IsReleased;
}

SAPI bool IsMouseDown(Buttons key)
{
	if (!State.IsInitialized) return false;
	return State.MouseCur.Buttons[(int)key];
}

SAPI bool WasMouseDown(Buttons key)
{
	if (!State.IsInitialized) return false;
	return State.MousePrev.Buttons[(int)key];
}

SAPI int GetMouseWheelDelta()
{
	if (!State.IsInitialized) return false;
	return State.MouseCur.MouseDelta;
}

SAPI MousePos GetMousePosition()
{
	if (!State.IsInitialized) return { 0, 0 };
	return State.MouseCur.Pos;
}

SAPI MousePos GetPrevMousePosition()
{
	if (!State.IsInitialized) return { 0, 0 };
	return State.MousePrev.Pos;
}

}
}
