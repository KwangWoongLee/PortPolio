#pragma once
#include "stdafx.h"

class MoveComponent : public Component
{
public:
	MoveComponent(class Actor* owner, int updateOrder = 10);

	void Update(float deltaTime) override;

	float	GetForwardSpeed() const { return mForwardSpeed; }
	void	SetForwardSpeed(float speed) { mForwardSpeed = speed; }

	float	GetUpwardSpeed() const { return mUpwardSpeed; }
	void	SetUpwardSpeed(float speed) { mUpwardSpeed = speed; }

	float	GetMovingSpeed() const { return mMovingSpeed; }
	void	SetMovingSpeed(float speed) { mMovingSpeed = speed; }

private:
	float	mForwardSpeed;
	float	mUpwardSpeed;

	float	mMovingSpeed;
};

