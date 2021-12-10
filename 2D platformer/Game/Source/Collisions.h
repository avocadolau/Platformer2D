#ifndef __MODULE_COLLISIONS_H__
#define __MODULE_COLLISIONS_H__

#define MAX_COLLIDERS 500

#include "Module.h"
#include "Collider.h"
#include "List.h"

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

	List<Collider*> collidersList;
	bool matrix[Collider::Type::MAX][Collider::Type::MAX];

};

#endif // __MODULE_COLLISIONS_H__