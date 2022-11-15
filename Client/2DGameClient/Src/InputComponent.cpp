#include "stdafx.h"

InputComponent::InputComponent(Actor* owner)
	:MoveComponent(owner)
	, mForwardKey(0)
	, mBackKey(0)
	, mUpKey(0)
	, mDownKey(0)
{
}

void InputComponent::ProcessInput(const class KeyboardState& keyState)
{
	if (keyState.GetKeyState(SDL_Scancode(mForwardKey)) == EReleased ||
		keyState.GetKeyState(SDL_Scancode(mForwardKey)) == ENone ||
		keyState.GetKeyState(SDL_Scancode(mBackKey)) == EReleased ||
		keyState.GetKeyState(SDL_Scancode(mBackKey)) == ENone ||
		keyState.GetKeyState(SDL_Scancode(mUpKey)) == EReleased ||
		keyState.GetKeyState(SDL_Scancode(mUpKey)) == ENone ||
		keyState.GetKeyState(SDL_Scancode(mDownKey)) == EReleased ||
		keyState.GetKeyState(SDL_Scancode(mDownKey)) == ENone
		)
	{
		mOwner->SetMovingState(Actor::MovingState::EStop);
	}

	float ForWardSpeed = 0.0f;
	if (keyState.GetKeyValue(SDL_Scancode(mForwardKey)))
	{
		ForWardSpeed += GetMovingSpeed();
		SetUpwardSpeed(0);
		mOwner->SetMovingState(Actor::MovingState::ERight);
	}

	if (keyState.GetKeyValue(SDL_Scancode(mBackKey)))
	{
		ForWardSpeed -= GetMovingSpeed();
		SetUpwardSpeed(0);
		mOwner->SetMovingState(Actor::MovingState::ELeft);
	}
	SetForwardSpeed(ForWardSpeed);
	


	float UpWardSpeed = 0.0f;
	if (keyState.GetKeyValue(SDL_Scancode(mUpKey)))
	{
		UpWardSpeed -= GetMovingSpeed();
		SetForwardSpeed(0);
		mOwner->SetMovingState(Actor::MovingState::EUp);
	}

	if (keyState.GetKeyValue(SDL_Scancode(mDownKey)))
	{
		UpWardSpeed += GetMovingSpeed();
		SetForwardSpeed(0);
		mOwner->SetMovingState(Actor::MovingState::EDown);
	}
	SetUpwardSpeed(UpWardSpeed);

}
