#ifndef __MODULE_COLLISIONS_H__
#define __MODULE_COLLISIONS_H__

#define MAX_COLLIDERS 350

#include "Module.h"
#include "Collider.h"

class Collisions : public Module
{
public:
	Collisions();
	virtual ~Collisions();

	bool Awake();
	bool Start();
	bool PreUpdate();
	bool Update(float dt);
	bool PostUpdate();

	bool CleanUp();

	Collider* AddCollider(SDL_Rect rect, Collider::Type type, Module* listener = nullptr);
	void RemoveCollider(Collider* collider);
	void DebugDraw();

public:

	Collider* colliders[MAX_COLLIDERS] = { nullptr };
	bool matrix[Collider::Type::MAX][Collider::Type::MAX];
	bool debug = false;

};

#endif // __MODULE_COLLISIONS_H__