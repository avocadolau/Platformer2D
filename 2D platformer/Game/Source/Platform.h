#ifndef __PLATFORM_H__
#define __PLATFORM_H__

#pragma once

#include "Collider.h"
#include "Collisions.h"
#include "App.h"

#include "SDL/include/SDL_Rect.h"

#define FALLING_TIME 5

struct Platform
{
	Platform(SDL_Rect rec, bool canFall, int tileWidth)
	{
		
	}
	~Platform()
	{
		app->collisions->RemoveCollider(col);
		app->collisions->RemoveCollider(down);
	}

	void AddTile()
	{

	}

	Collider* col;
	Collider* down;
	bool canFall;
	int timeToFall = FALLING_TIME;
	int tiles;
	int tileWidth;
};

#endif // !__PLATFORM_H__
