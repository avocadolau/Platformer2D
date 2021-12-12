#ifndef __PLAYER_ATACK_H__
#define __PLAYER_ATACK_H__

#include "App.h"
#include "Module.h"
#include "SceneGame.h"

#include "Defs.h"
#include "Point.h"

struct PlayerAtack
{
	fPoint pos;
	int dim = 20;
	float vel;
	Collider* col;

	PlayerAtack() {}

	PlayerAtack::PlayerAtack(fPoint pos_, float vel_) : pos(pos_)
	{
		vel = vel_ * 2;
		SDL_Rect rec = { pos.x,pos.y,dim,dim };
		col = app->collisions->AddCollider(rec, Collider::Type::PATACK, nullptr);
	}

	~PlayerAtack()
	{
		app->collisions->RemoveCollider(col);
	}

	void Update(float dt)
	{
		pos.x += vel * dt;
		col->rect.x = (int)pos.x;
	}
	
	void Draw()
	{
		SDL_Rect rec = col->rect;
		app->render->DrawRectangle(col->rect, 122, 0, 255, 200, true, true);
	}
	
};


#endif // !__PLAYER_ATACK_H__


