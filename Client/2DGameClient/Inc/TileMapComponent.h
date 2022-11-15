#pragma once
#include "stdafx.h"

class TileMapComponent : public SpriteComponent
{
public:
	TileMapComponent(Actor* owner, int drawOrder);

	void Draw(SDL_Renderer* renderer) override;

	

	void SetSizeX(int x) { mSizeX = x; }
	void SetSizeY(int y) { mSizeY = y; }
	void SetTileSizeX(int x) { mTileSizeX = x; }
	void SetTileSizeY(int y) { mTileSizeY = y; }

	void SetTileNumber(int num) { mTileNum = num; }
private:
	int mSizeX;
	int mSizeY;
	int mTileSizeX;
	int mTileSizeY;
	int mTileNum;
};

