#include "stdafx.h"

Tile::Tile(Game* game, int tilenum)
	: Actor(game)
	, mCollider(nullptr)
{
	TileMapComponent* tile = new TileMapComponent(this, 15);
	tile->SetSizeX(8);
	tile->SetSizeY(24);
	tile->SetTileNumber(tilenum);
	tile->SetTexture(game->GetTexture("Assets/Tiles1.bmp"));

	CollisionComponent* collider = new CollisionComponent(this);
	collider->SetHalfHeight(32);
	collider->SetHalfWidth(32);

	mCollider = collider;

	game->AddTile(this);
}

Tile::~Tile()
{
	GetGame()->RemoveTile(this,GetPosition());
}

void Tile::UpdateActor(float deltaTime)
{
	if (GetState() == State::ETempDie)
	{
		mDeathTimer += deltaTime;

		if (isDead())
		{
			SetState(State::EDead);
		}
	}
}
