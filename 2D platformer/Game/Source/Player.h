#ifndef __PLAYER_H
#define __PLAYER_H

#pragma once
#include "Entity.h"
#include "Collider.h"
#include "Animation.h"
#include "Textures.h"
#include "Timer.h"

#define COOLDOWN 500

class Player : public Entity
{
public:
	
	Player();
	~Player();

	//bool Awake(pugi::xml_node& config);
	bool Awake(pugi::xml_node&);
	bool Start();
	bool Update(float dt);
	bool Draw(Render* render);
	bool CleanUp();
	
	void Enable();
	void Disable();

	bool Load(pugi::xml_node&);
	bool Save(pugi::xml_node&) const;

	bool CreateColliders();
	void OnCollision(Collider* c1, Collider* c2);


public:

	fPoint pos;
	fPoint vel;
	fPoint maxVel;
	Collider* collider;
	iPoint dim = { 35,64 };

	float gravity;
	int jumps;
	int lives;
	bool alive = true;

	Collider* colUp;
	Collider* colDown;
	Collider* colRight;
	Collider* colLeft;
	Collider* lastGround;
	bool up = true;
	bool down = true;
	bool left = true;
	bool right = true;
	bool godMode = false;

	SDL_Texture* sprites;
	Animation* currentAnim = nullptr;
	Animation idle, run, jump, death;
	float animSpeed;

	Timer shotCoolDown;

};

#endif