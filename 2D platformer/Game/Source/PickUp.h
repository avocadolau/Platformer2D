#ifndef __PICKUP_H__
#define __PICKUP_H__

#include "List.h"
#include "Point.h"

#include "App.h"
#include "Collider.h"
#include "Collisions.h"
#include "Render.h"
#include "SceneGame.h"

struct PickUp
{
	iPoint pos;
	Collider* col;
	int dim = 16;

	PickUp()
	{

	}

	PickUp(iPoint pos_) :pos(pos_)
	{
		SDL_Rect rec = { pos.x,pos.y,dim,dim };
		col = app->collisions->AddCollider(rec, Collider::Type::PICKUP, nullptr);
	}

	~PickUp()
	{
		app->collisions->RemoveCollider(col);
	}

	bool Draw()
	{
		app->render->DrawCircle((int)pos.x, (int)pos.y, dim, 199, 128, 241, 255, true);
		SDL_Rect rec = col->rect;
		app->render->DrawRectangle(rec, 249, 219, 73, 255, true, true);
		return true;
	}

	
};

#endif // !__PICKUP_H__
