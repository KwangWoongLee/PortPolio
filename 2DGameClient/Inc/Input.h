#pragma once
#include "stdafx.h"


// The different button states
enum ButtonState
{
	ENone,
	EPressed,
	EReleased,
	EHeld
};


class KeyboardState
{
public:
	friend class Input;
	// Get just the boolean true/false value of key
	bool GetKeyValue(SDL_Scancode keyCode) const;
	// Get a state based on current and previous frame
	ButtonState GetKeyState(SDL_Scancode keyCode) const;
private:
	const Uint8* mCurrState;
	Uint8 mPrevState[SDL_NUM_SCANCODES];
};

class Input
{
public:
	bool Init();
	void PrepareForUpdate();
	const KeyboardState& GetKeyboardState() const { return mKeyboardState; }

private:
	KeyboardState mKeyboardState;

};

