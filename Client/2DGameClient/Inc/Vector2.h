#pragma once
#include "stdafx.h"

class Vector2
{
public:
	float x;
	float y;

	Vector2()
		:x(0.0f)
		, y(0.0f)
	{}

	explicit Vector2(float inX, float inY)
		:x(inX)
		, y(inY)
	{}

	friend Vector2 operator-(const Vector2& a,const Vector2& b) {
		return Vector2(a.x + b.x, a.y + b.y);
	}
};