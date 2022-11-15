#pragma once
#include "stdafx.h"

class SpriteComponent : public Component
{
public:
	SpriteComponent(class Actor* owner, int drawOrder);
	~SpriteComponent();
		
	virtual void Draw(SDL_Renderer* renderer);
	
	//Getter, Setter
	virtual void SetTexture(SDL_Texture* texture);

	SDL_Texture* GetTexture() const { return mTexture; }
	int GetTextureHeight() const { return mTextureHeight; }
	int GetTextureWidth() const { return mTextureWidth; }

	int GetDrawOrder() const { return mDrawOrder; }


protected:
	//그리기 순서변수
	int				mDrawOrder;

	//스프라이트에 종속된 Texture 변수
	SDL_Texture*	mTexture;
	int				mTextureHeight;
	int				mTextureWidth;


};
