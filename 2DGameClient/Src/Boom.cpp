#include "stdafx.h"

Boom::Boom(Game* game, BoomPosition state, Vector2 pos)
	:Actor(game)
	, mCollider(nullptr)
	, mDeathTimer(0.f)
	, EBoomPosition(state)
{
	SetPosition(pos);

	AnimationComponent* anim = new AnimationComponent(this, 150);
	SDL_Texture* boomCenter = game->GetTexture("Assets/boom_center.bmp");
	SDL_Texture* boomLeft = game->GetTexture("Assets/boom_left.bmp");
	SDL_Texture* boomRight = game->GetTexture("Assets/boom_right.bmp");
	SDL_Texture* boomUp = game->GetTexture("Assets/boom_up.bmp");
	SDL_Texture* boomDown = game->GetTexture("Assets/boom_down.bmp");
	
	if (GetGame()->GetTileMapToType()[(pos.y) / 32][(pos.x) / 32] == Tile::TileType::ENone)
		switch (EBoomPosition)
		{
		case ECenter:
			anim->SetAnimTexture(boomCenter, 14);
			break;

		case ELeft:
			anim->SetAnimTexture(boomLeft, 9);
			break;

		case ERight:
			anim->SetAnimTexture(boomRight, 9);
			break;

		case EUp:
			anim->SetAnimTexture(boomUp, 9);
			break;

		case EDown:
			anim->SetAnimTexture(boomDown, 9);
			break;
		}

	CollisionComponent* collider = new CollisionComponent(this);
	collider->SetHalfHeight(anim->GetTextureHeight() * 0.5f);
	collider->SetHalfWidth(anim->GetTextureWidth() / 9 * 0.5f);

	mCollider = collider;

	game->AddBoom(this);
}

Boom::~Boom()
{
	GetGame()->RemoveBoom(this);
}

void Boom::UpdateActor(float deltaTime)
{

	//Boom Actor ÀÇ ÀÚµ¿ Á×À½Ã³¸®
	mDeathTimer += deltaTime;
	if (isTimeToDeath())
		SetState(Actor::State::EDead);

	for (auto player : GetGame()->GetPlayers())
	{
		float CollisionX, CollisionY;
		//ÆøÅº°ú Ãæµ¹Çß´Ù¸é,
		if (Intersect(*GetCollider(), *(player->GetCollider()), &CollisionX, &CollisionY))
		{
			player->SetState(Actor::State::EBubble);
		}
	}

	for (auto bomb : GetGame()->GetBombs())
	{
		float CollisionX, CollisionY;
		//ÆøÅº°ú Ãæµ¹Çß´Ù¸é,
		if (Intersect(*GetCollider(), *(bomb->GetCollider()), &CollisionX, &CollisionY))
		{
			SetState(Actor::State::EDead);
			bomb->SetState(Actor::State::EDead);
		}
	}

	for (auto tile : GetGame()->GetTiles())
	{
		float CollisionX, CollisionY;
		//ÆøÅº°ú Ãæµ¹Çß´Ù¸é,
		if (Intersect(*GetCollider(), *(tile->GetCollider()), &CollisionX, &CollisionY))
		{
			SetState(Actor::State::EDead);
			if (tile->GetTileType() == Tile::TileType::EBush)
			{
				tile->SetState(Actor::State::ETempDie);
			}
		}
	}

}

