#pragma once
#include "stdafx.h"

class Player : public Actor
{
public:

	Player(class Game* game);
	~Player();

	virtual void UpdateActor(float deltaTime) override;
	
	void ActorInput(const class KeyboardState& keyState) override;

	Vector2 SearchBombPosition(const Vector2& ownerPos);

	CollisionComponent* GetCollider() const { return mCollider; }

	void SetBubbleToLive();

	bool isBubbleTimeOut() { return mDeathTimer >= 3.0f; }

	bool isDead() { return mDeathTimer >= 1.1f; }

private:
	CollisionComponent* mCollider;
	float mDeathTimer;
};

