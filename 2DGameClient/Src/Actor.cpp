#include "stdafx.h"

Actor::Actor(Game* game)
	:mState(State::EActive)
	,mMovingState(MovingState::ENone)
	, mPosition(0.f,0.f)
	, mScale(1.0f)
	, mGame(game)
{
	mGame->AddActor(this);
}

Actor::~Actor()
{
	mGame->RemoveActor(this);

	while (!mComponents.empty())
	{
		delete mComponents.back();
	}
}

void Actor::Update(float deltaTime)
{
	if (mState != State::EDead)
	{
		UpdateComponents(deltaTime);
		UpdateActor(deltaTime);
	}
}

void Actor::UpdateActor(float deltaTime)
{
	//virtual 가상함수, 실제 UpdateActor는 해당 Actor에서처리
}

void Actor::UpdateComponents(float deltaTime)
{
	for (auto component : mComponents)
	{
		component->Update(deltaTime);
	}
}

void Actor::ProcessInput(const class KeyboardState& keyState)
{
	if (mState == State::EActive)
	{
		for (auto comp : mComponents)
		{
			comp->ProcessInput(keyState);
		}

		ActorInput(keyState);
	}
}

void Actor::ActorInput(const class KeyboardState& keyState)
{
}

void Actor::AddComponent(Component* inComponent)
{
	//Component 그리기 순서를 위한 순서 정렬
	int myOrder = inComponent->GetUpdateOrder();
	auto iter = mComponents.begin();
	for (;
		iter != mComponents.end();
		++iter)
	{
		if (myOrder < (*iter)->GetUpdateOrder())
		{
			break;
		}
	}

	mComponents.insert(iter, inComponent);
}

void Actor::RemoveComponent(Component* inComponent)
{
	auto iter = std::find(mComponents.begin(), mComponents.end(), inComponent);
	if (iter != mComponents.end())
	{
		mComponents.erase(iter);
	}
}
