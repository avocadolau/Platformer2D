#ifndef __PLATFORM_H__
#define __PLATFORM_H__

#pragma once

#include "Collider.h"
#include "Collisions.h"
#include "App.h"
#include "Player.h"
#include "Point.h"
#include "Window.h"
#include "Module.h"
#include "SceneGame.h"
#include "Render.h"

#include "SDL/include/SDL_Rect.h"

#define FALLING_TIME 333

struct Platform
{
	Platform(iPoint pos)
	{
		this->pos = { (float)pos.x,(float)pos.y };
		SDL_Rect rec = { pos.x + 16,pos.y + 16,64 + 32,64 + 32 };
		col = app->collisions->AddCollider(rec, Collider::Type::GROUND, nullptr);
			
	}
	~Platform()
	{
		app->collisions->RemoveCollider(col);
	}

	void Update(float dt)
	{
		if (fall == false)
		{
			if (app->player->godMode==false)
				fall = col->Intersects(app->player->colDown->rect);
		}
		if (fall == true)
		{
			if (timeToFall > 0)timeToFall -= dt;
			else
			{
				col->type = Collider::Type::NONE;
				//vel += app->player->gravity * dt;
				vel = app->player->maxVel.y;
				if (vel > app->player->maxVel.y) vel = app->player->maxVel.y;
				pos.y += vel * dt;
				col->rect.y += vel * dt;

				uint winx;
				uint winy;
				app->win->GetWindowSize(winx, winy);
				if (pos.y < winy * 1.5f)active = false;
			}
		}
	}

	void AddTile()
	{

	}

	Collider* col;
	bool fall = false;
	bool active = true;
	int timeToFall = FALLING_TIME;
	int tiles;
	int tileWidth;
	float vel = 0;
	fPoint pos;
};

#endif // !__PLATFORM_H__
