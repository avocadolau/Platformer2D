#ifndef __PICKUP_H__
#define __PICKUP_H__

#include "List.h"
#include "Point.h"

#include "App.h"
#include "Entity.h"
#include "Collider.h"
#include "Collisions.h"
#include "Render.h"
#include "SceneGame.h"

class Coin: public Entity
{
public:
	Coin(uint32 id, SDL_Rect bounds);
	virtual ~Coin();

	bool Update(float dt);

	bool Draw(Render* render);

	void OnCollision(Collider* c1, Collider* c2);

	bool CleanUp();
public:

	fPoint pos;
	Collider* col;
	int dim = 16;

	SDL_Texture* texture;
	unsigned int fx;

	
};

#endif // !__PICKUP_H__
