#pragma once
#include "stdafx.h"

class Block : public Actor
{
public:
	Block(class Game* game,int tilenum);
	~Block();

	void UpdateActor(float deltaTime) override;

};

