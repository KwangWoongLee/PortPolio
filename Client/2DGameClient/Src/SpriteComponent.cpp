#include "stdafx.h"

SpriteComponent::SpriteComponent(Actor* owner, int drawOrder)
	:Component(owner)
	,mTexture(nullptr)
	,mDrawOrder(drawOrder)
	,mTextureHeight(0)
	,mTextureWidth(0)
{
	mOwner->GetGame()->AddSprite(this);
}

SpriteComponent::~SpriteComponent()
{
	mOwner->GetGame()->RemoveSprite(this);
}

void SpriteComponent::Draw(SDL_Renderer* renderer)
{
	if (mTexture)
	{
		//���� actor�� ������ ���� texture ��ġ
		SDL_Rect r;
		r.w = static_cast<int>(mTextureWidth * mOwner->GetScale());
		r.h = static_cast<int>(mTextureHeight * mOwner->GetScale());
		r.x = static_cast<int>(mOwner->GetPosition().x - r.w / 2);
		r.y = static_cast<int>(mOwner->GetPosition().y - r.h / 2);


		SDL_RenderCopy(renderer,
			mTexture,
			nullptr,
			&r);
	}
}

void SpriteComponent::SetTexture(SDL_Texture* texture)
{
	mTexture = texture;

	//�ش� texture�� width�� height ������ ����
	SDL_QueryTexture(texture, nullptr, nullptr, &mTextureWidth, &mTextureHeight);
}
