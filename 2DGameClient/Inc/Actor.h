#pragma once
#include "stdafx.h"

class Actor
{
public:
	enum class State
	{
		EActive,
		EBubble,
		ETempDie,
		EDead
	};

	enum class MovingState
	{
		ENone,
		EIdle,
		ELeft,
		ERight,
		EUp,
		EDown,
		EStop
	};

public:
	Actor(class Game* game);
	virtual ~Actor();

	//Game Update에서 호출하는 Update(UpdateGame -> 각 Actor Update)
	void Update(float deltaTime);
	
	//Actor Update에서 호출하는 Update(Actor Update-> UpdateActor)
	virtual void UpdateActor(float deltaTime);
	// Actor에 붙어있는 모든 Component들 업데이트
	void UpdateComponents(float deltaTime);

	
	void ProcessInput(const class KeyboardState& keyState);

	virtual void ActorInput(const class KeyboardState& keyState);

	//Getter , Setter
	State	GetState() const { return mState; }
	void	SetState(State state) { mState = state; }

	MovingState	GetMovingState() const { return mMovingState; }
	void		SetMovingState(MovingState state) { mMovingState = state; }

	float	GetScale() const		{ return mScale; }
	void	SetScale(float scale)	{ mScale = scale; }

	Vector2 GetPosition() const { return mPosition; }
	void	SetPosition(const Vector2& inPosition) { mPosition = inPosition; }

	
	Game*	GetGame() const { return mGame; }

	//Component 추가 및 삭제
	void AddComponent(class Component* inComponent);
	void RemoveComponent(class Component* inComponent);
	
private:
	State	mState;
	MovingState	mMovingState;
	Vector2 mPosition;
	float	mScale;

	//actor 객체가 소유한 component를 담는 변수
	std::vector<class Component*> mComponents;
	//의존성 주입
	Game*	mGame;

};

