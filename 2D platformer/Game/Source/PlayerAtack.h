#ifndef __PLAYER_ATACK_H__
#define __PLAYER_ATACK_H__

#include "Module.h"
#include "Animation.h"
#include "SceneGame.h"

#include "Defs.h"
#include "Point.h"

class PlayerAtack : public Module
{
public:
	PlayerAtack();
	~PlayerAtack();

	bool Update(float dt);
	bool PostUpdate();

	void OnCollision();
	bool CleanUp();
	

public:
	fPoint pos;
	fPoint vel;
	float maxSpeed;
	Collider* col;
};


#endif // !__PLAYER_ATACK_H__


