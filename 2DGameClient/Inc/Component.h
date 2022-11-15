#pragma once
#include "stdafx.h"

class Component
{
public:
	Component(class Actor* owner, int updateOrder = 100);
	virtual ~Component();

	virtual void ProcessInput(const class KeyboardState& keyState);

	virtual void Update(float deltaTime);


	int GetUpdateOrder() const { return mUpdateOrder; }

protected:
	class Actor* mOwner;

	int mUpdateOrder;

};

