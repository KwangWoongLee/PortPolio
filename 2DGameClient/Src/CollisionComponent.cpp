#include "stdafx.h"

CollisionComponent::CollisionComponent(Actor* owner)
	:Component(owner)
	, mHalfHeight(0.0f)
	, mHalfWidth(0.0f)
{
}


bool Intersect(const CollisionComponent& me,const CollisionComponent& other, float* pCollX, float* pCollY)
{
	float fmeX = me.GetPosition().x;
	float fotherX = other.GetPosition().x;
	float fDistanceX = fabsf(fmeX - fotherX);
	float fRadCX = other.GetHalfWidth() + me.GetHalfWidth();


	float fmeY = me.GetPosition().y;
	float fotherY = other.GetPosition().y;
	float fDistanceY = fabsf(fmeY - fotherY);
	float fRadCY = other.GetHalfHeight() + me.GetHalfHeight();

	if (fDistanceX >= fRadCX || fDistanceY >= fRadCY)
		return false;

	*pCollX = fRadCX - fDistanceX;
	*pCollY = fRadCY - fDistanceY;

	return true;
}

