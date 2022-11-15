#include "stdafx.h"

TileMapComponent::TileMapComponent(Actor* owner, int drawOrder)
	:SpriteComponent(owner, drawOrder)
	, mSizeX(0)
	, mSizeY(0)
	, mTileNum(0)
{
	SetTileSizeX(32);
	SetTileSizeY(32);
}

void TileMapComponent::Draw(SDL_Renderer* renderer)
{
	if (mTexture)
	{
		SDL_Rect src;
		src.w = static_cast<int>(mTileSizeX);
		src.h = static_cast<int>(mTileSizeY);
		src.x = static_cast<int>(mTileNum % mSizeX * 32);
		src.y = static_cast<int>(mTileNum / mSizeX * 32);
		
		//소유 actor의 정보에 따라 texture 배치
		
		SDL_Rect dsc;
		dsc.w = static_cast<int>(mTileSizeX);
		dsc.h = static_cast<int>(mTileSizeY);
		dsc.x = static_cast<int>(mOwner->GetPosition().x - 16);
		dsc.y = static_cast<int>(mOwner->GetPosition().y - 16);

		SDL_RenderCopy(renderer,
			mTexture,
			&src,
			&dsc);
	}
}
