#ifndef __COLLIDER_H__
#define __COLLIDER_H__

#pragma once
#include "SDL/include/SDL_Rect.h"
#include "Point.h"
#define MAX_LISTENERS 5

class Module;

struct Collider
{
	enum Type
	{
		NONE,
		LISTENER,
		GROUND,
		PLAYER,
		WIN,
		BORDER,
		MAX
	};

	//Methods
	Collider();
	Collider(SDL_Rect rectangle, Type type, Module* listener = nullptr);

	~Collider();

	void SetPos(int x, int y);

	iPoint GetPos() { return iPoint{ rect.x,rect.y }; }

	bool Intersects(const SDL_Rect& r) const;
	bool Inside(const SDL_Rect& r) const;

	void AddListener(Module* listener);

	//Variables
	SDL_Rect rect;
	bool pendingToDelete = false;
	Type type;
	Module* listeners[MAX_LISTENERS] = { nullptr };
	int level;
};

#endif // !__COLLIDER_H__


