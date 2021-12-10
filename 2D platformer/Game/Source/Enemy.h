#ifndef __ENEMY_H__
#define __ENEMY_H__

#include "Module.h"
#include "Collider.h"
#include "Point.h"
#include "DynArray.h"

class Enemy : public Module
{
	Enemy();
	~Enemy();

	bool Awake();
	bool Start();
	bool PreUpdate();
	bool Update(float dt);
	bool PostUpdate();
	bool CleanUp();
	bool LoadState(pugi::xml_node&);
	bool SaveState(pugi::xml_node&) const;

	void OnCollision(Collider* c1, Collider* c2);

public:

	iPoint pos;
	iPoint dim;

	Collider* col;
	Collider* up;
	Collider* down;
	Collider* detector;

	DynArray<iPoint> path;
};

#endif // !__ENEMY_H__