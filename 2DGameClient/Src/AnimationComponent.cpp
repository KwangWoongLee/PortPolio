#include "stdafx.h"

AnimationComponent::AnimationComponent(Actor* owner, int drawOrder)
	:SpriteComponent(owner, drawOrder)
	, mCurrFrame(0.0f)
	, mAnimFPS(10.0f)
	, mFrameX(1)
{
}

void AnimationComponent::Update(float deltaTime)
{
	SpriteComponent::Update(deltaTime);

	if (mOwner->GetMovingState() == Actor::MovingState::EIdle)
	{
		mAnimTexture = mAnimTextures.find("idle")->second;
		mFrameX = 1;
	}
	else if (mOwner->GetMovingState() == Actor::MovingState::EDown)
	{
		mAnimTexture = mAnimTextures.find("down")->second;
		mFrameX = 8;
	}
	else if (mOwner->GetMovingState() == Actor::MovingState::EUp)
	{
		mAnimTexture = mAnimTextures.find("up")->second;
		mFrameX = 8;
	}
	else if (mOwner->GetMovingState() == Actor::MovingState::ERight)
	{
		mAnimTexture = mAnimTextures.find("right")->second;
		mFrameX = 6;
	}
	else if (mOwner->GetMovingState() == Actor::MovingState::ELeft)
	{
		mAnimTexture = mAnimTextures.find("left")->second;
		mFrameX = 6;
	}

	if (mOwner->GetState() == Actor::State::EBubble)
	{
		mAnimTexture = mAnimTextures.find("bubble")->second;
		mFrameX = 16;
	}
	else if (mOwner->GetState() == Actor::State::ETempDie)
	{
		mAnimTexture = mAnimTextures.find("tempdie")->second;
		mFrameX = 13;
	}

	if (mOwner->GetMovingState() == Actor::MovingState::EStop && mOwner->GetState() == Actor::State::EActive)
	{
		mCurrFrame = 0.f;
	}
	else 
	{
		mCurrFrame += mAnimFPS * deltaTime;
	}
	
	//모든 texture animation이 끝나면 처음 animation으로 변경
	while (mCurrFrame >= mFrameX)
	{
		mCurrFrame -= mFrameX;
	}
	
	// mCurrFrame에 맞는 texture 적용
	SetTexture(mAnimTexture);
}

void AnimationComponent::Draw(SDL_Renderer* renderer)
{
	if (mTexture)
	{
		SDL_Rect src;
		src.w = static_cast<int>(mTextureWidth / mFrameX );
		src.h = static_cast<int>(mTextureHeight);
		src.x = static_cast<int>(mTextureWidth / mFrameX * static_cast<int>(mCurrFrame));
		src.y = static_cast<int>(0);

		//소유 actor의 정보에 따라 texture 배치
		SDL_Rect dsc;
		dsc.w = static_cast<int>(mTextureWidth / mFrameX * mOwner->GetScale());
		dsc.h = static_cast<int>(mTextureHeight * mOwner->GetScale());
		dsc.x = static_cast<int>(mOwner->GetPosition().x - dsc.w / 2);
		dsc.y = static_cast<int>(mOwner->GetPosition().y - dsc.h / 2);

		SDL_RenderCopy(renderer,
			mTexture,
			&src,
			&dsc);
	}
}

void AnimationComponent::SetAnimTexture(SDL_Texture* texture, int frameX)
{
	mAnimTexture = texture;

	mFrameX = frameX;

	mCurrFrame = 0.0f;

	
	SetTexture(mAnimTexture);
}

void AnimationComponent::AddAnimTextures(std::string strTexture, SDL_Texture* animTexture)
{
	mAnimTextures.insert(std::pair<std::string, SDL_Texture*>(strTexture, animTexture));
}
