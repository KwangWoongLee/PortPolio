#pragma once
#include "stdafx.h"

class Bomb;

class Boom : public Actor
{
public:
	enum BoomPosition
	{
		ECenter,
		ELeft,
		ERight,
		EUp,
		EDown
	} EBoomPosition;

	Boom(class Game* game, BoomPosition state, Vector2 pos);
	~Boom();

	void UpdateActor(float deltaTime) override;

	CollisionComponent* GetCollider() const { return mCollider; }



private:
	bool	isTimeToDeath() {return mDeathTimer >= 0.8f;};
	float	mDeathTimer;

	CollisionComponent* mCollider;
};

