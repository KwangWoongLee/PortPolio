#include "stdafx.h"

Block::Block(Game* game, int tilenum)
	: Actor(game)
{
	TileMapComponent* tile = new TileMapComponent(this, 15);
	tile->SetSizeX(8);
	tile->SetSizeY(24);
	tile->SetTileNumber(tilenum);
	tile->SetTexture(game->GetTexture("Assets/Tiles.png"));
}

Block::~Block()
{
}

void Block::UpdateActor(float deltaTime)
{

}
