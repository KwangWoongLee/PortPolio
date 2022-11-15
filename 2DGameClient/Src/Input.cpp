#include "stdafx.h"
#include "Input.h"

bool KeyboardState::GetKeyValue(SDL_Scancode keyCode) const
{
	return mCurrState[keyCode] == 1;
}

ButtonState KeyboardState::GetKeyState(SDL_Scancode keyCode) const
{
	if (mPrevState[keyCode] == 0)
	{
		if (mCurrState[keyCode] == 0)
		{
			return ENone;
		}
		else
		{
			return EPressed;
		}
	}
	else // Prev state must be 1
	{
		if (mCurrState[keyCode] == 0)
		{
			return EReleased;
		}
		else
		{
			return EHeld;
		}
	}
}

bool Input::Init()
{
	mKeyboardState.mCurrState = SDL_GetKeyboardState(NULL);
	// Clear previous state memory
	memset(mKeyboardState.mPrevState, 0,
		SDL_NUM_SCANCODES);

	return true;
}

void Input::PrepareForUpdate()
{
	memcpy(mKeyboardState.mPrevState,
		mKeyboardState.mCurrState,
		SDL_NUM_SCANCODES);
}
