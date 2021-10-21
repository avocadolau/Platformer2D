#ifndef __COLLISIONS_H__
#define __COLLISIONS_H__

#define MAX_COLLIDERS 350
#pragma once
#include "Module.h"
#include "Collider.h"


class Collisions : public Module
{
public:

	Collisions();
	~Collisions();

	bool PreUpdate();
	bool Update();
	bool PostUpdate();
	bool CleanUp();
	Collider* AddCollider(SDL_Rect rect, Collider::Type type, Module* listener = nullptr);
	void RemoveCollider(Collider* collider);
	void DebugDraw();

private:
	Collider* colliders[MAX_COLLIDERS] = { nullptr };
	bool matrix[Collider::Type::MAX][Collider::Type::MAX];
	bool debug = false;
};

#endif