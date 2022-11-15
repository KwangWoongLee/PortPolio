#include "stdafx.h"

MoveComponent::MoveComponent(Actor* owner, int updateOrder)
	:Component(owner, updateOrder)
	,mForwardSpeed(0.f)
	,mUpwardSpeed(0.f)
	,mMovingSpeed(0.f)
{
}

void MoveComponent::Update(float deltaTime)
{
	Vector2 pos = mOwner->GetPosition();

	if (abs(mForwardSpeed) > 0.001f)
	{
		pos.x += mForwardSpeed * mMovingSpeed  *deltaTime;
	}

	if (abs(mUpwardSpeed) > 0.001f)
	{
		pos.y += mUpwardSpeed * mMovingSpeed * deltaTime;
	}

	mOwner->SetPosition(pos);
}
