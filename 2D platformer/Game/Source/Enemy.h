#ifndef __ENEMY_H__
#define __ENEMY_H__

#include "Module.h"
#include "Collider.h"
#include "Point.h"
#include "DynArray.h"
#include "Animation.h"

#include "SDL_image/include/SDL_image.h"

class Enemy : public Module
{
public:

	enum Type
	{
		FLY,
		WALK
	};

	enum State
	{
		ALIVE,
		DEATH
	};

	Enemy(int id,iPoint dim_, SDL_Rect pDetector, iPoint lim1, iPoint lim2, Type type);
	~Enemy();

	bool Awake();
	bool Start();
	bool PreUpdate();
	bool Update(float dt);
	bool PostUpdate();
	bool CleanUp();


	bool LoadState(pugi::xml_node&);
	bool SaveState(pugi::xml_node&) const;
	bool DrawPath();

	void SetPosition(iPoint pos);
	void OnCollision(Collider* c1, Collider* c2);

	void Fly(float dt);
	void Walk(float dt);

public:
	int tileDim;
	int id;

	fPoint pos;
	iPoint dim;
	iPoint coord;

	Collider* col;
	Collider* up;
	Collider* down;
	Collider* detector;

	Animation idleAnim, death;
	Animation* currentAnim;

	// MOVEMENT LIMITS
	iPoint lim1;
	iPoint lim2;
	iPoint lastLim;

	State state = ALIVE;
	Type type;

	const DynArray<iPoint>* path;
	//DynArray<iPoint> path;
	bool pNear = false;
};


#endif