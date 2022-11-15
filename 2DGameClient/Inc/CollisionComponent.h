#pragma once
#include "stdafx.h"

class CollisionComponent : public Component
{
public:
	CollisionComponent(Actor* owner);

	const Vector2& GetPosition() const { return mOwner->GetPosition(); }

	float GetHalfWidth() const { return mHalfWidth; }
	void SetHalfWidth(float width) { mHalfWidth = width * 0.5f; }

	float GetHalfHeight() const { return mHalfHeight; }
	void SetHalfHeight(float height) { mHalfHeight = height * 0.5f; }

	

private:
	float mHalfWidth;
	float mHalfHeight;



};

bool Intersect(const CollisionComponent& a,const CollisionComponent& b, float* pCollX, float* pCollY);
