#ifndef __PLAYER_ATACK_H__
#define __PLAYER_ATACK_H__

#include "App.h"
#include "Module.h"
#include "SceneGame.h"
#include "Entity.h"

#include "Defs.h"
#include "Point.h"

class PlayerAtack : public Entity
{
public:
	PlayerAtack(SDL_Rect bounds) :Entity(EntityType::ATACK)
	{
		name.Create("playerAtack");
		active = true;
		id = 0;
		vel = app->player->maxVel.x * 2;
		SDL_Rect rec = { pos.x,pos.y,dim,dim };
		col = app->collisions->AddCollider(rec, Collider::Type::PATACK, app->entityManager, this);
	}

	~PlayerAtack() {
		app->collisions->RemoveCollider(col);
	}

	bool Update(float dt)
	{
		pos.x += vel * dt;
		col->rect.x = (int)pos.x;
		return true;
	}

	bool Draw(Render* render)
	{
		SDL_Rect rec = col->rect;
		bool draw = app->render->DrawRectangle(col->rect, 122, 0, 255, 200);
		
		return true;
	}

	void OnCollision(Collider* c1, Collider* c2)
	{
		if (c2->type!=Collider::Type::PLAYER)
			toDestroy = true;
	}
public:
	fPoint pos;
	int dim = 20;
	float vel;
	Collider* col;
};

#endif // !__PLAYER_ATACK_H__


