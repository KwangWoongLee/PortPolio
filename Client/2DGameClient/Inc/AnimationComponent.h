#pragma once
#include "stdafx.h"

class AnimationComponent : public SpriteComponent
{
public:
	AnimationComponent(class Actor* owner, int drawOrder = 100);
	
	void Update(float deltaTime);

	virtual void Draw(SDL_Renderer* renderer) override;
	
	// Getter, Setter
	void SetAnimTexture(SDL_Texture* texture, int frameX);

	void AddAnimTextures(std::string strTexture, SDL_Texture* mAnimTexture);

	float GetAnimFPS() const { return mAnimFPS; }
	void SetAnimFPS(float fps) { mAnimFPS = fps; }
private:
	// Texture Vector
	SDL_Texture* mAnimTexture;
	// �ൿ�� texture�� ���� map
	std::unordered_map<std::string, SDL_Texture*> mAnimTextures;
	// ���� ������
	float mCurrFrame;
	// �ִϸ��̼� �����Ӽ�
	int   mFrameX;
	// FPS
	float mAnimFPS;

};

