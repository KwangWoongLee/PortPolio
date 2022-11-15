#include "stdafx.h"

Component::Component(Actor* owner, int updateOrder)
	:mOwner(owner)
	,mUpdateOrder(updateOrder)
{
	mOwner->AddComponent(this);
}

Component::~Component()
{
	mOwner->RemoveComponent(this);
}

void Component::ProcessInput(const class KeyboardState& keyState)
{
}

void Component::Update(float deltaTime)
{
}
