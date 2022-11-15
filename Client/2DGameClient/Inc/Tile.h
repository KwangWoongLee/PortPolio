#pragma once
#include "stdafx.h"

class Tile : public Actor
{
public:

	enum class TileType
	{
		ENone,
		EBlock,
		EBush
	};

	Tile(class Game* game, int tilenum);
	~Tile();

	void UpdateActor(float deltaTime) override;

	TileType GetTileType() const { return mType; }
	void SetTileType(TileType type) { mType = type; }

	CollisionComponent* GetCollider() const { return mCollider; }

	bool isDead() { return mDeathTimer >= 0.2f; }

private:
	CollisionComponent* mCollider;
	TileType mType;

	float mDeathTimer;
};

