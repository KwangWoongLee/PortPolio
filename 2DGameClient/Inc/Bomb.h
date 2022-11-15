#pragma once
#include "stdafx.h"

class Bomb : public Actor
{
public:
	Bomb(class Game* game);
	~Bomb();

	void UpdateActor(float deltaTime) override;

	CollisionComponent* GetCollider() const { return mCollider; }



	bool	isFirstContact() const { return bFirstContact; }
	void	SetFirstContact() { bFirstContact = false; }

private:

	bool	isTimeToDeath() { return mDeathTimer >= 3.f; }
	bool	isDead() { return mDeathTimer >= 1.1f; }
	float	mDeathTimer;
	bool	bFirstContact;

	CollisionComponent* mCollider;
};

