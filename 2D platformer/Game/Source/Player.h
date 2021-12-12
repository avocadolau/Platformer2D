#ifndef __PLAYER_H
#define __PLAYER_H

#pragma once
#include "Module.h"
#include "Collider.h"
#include "Animation.h"
#include "Textures.h"
#include "Timer.h"

#define COOLDOWN 500

class Player : public Module
{
public:
	
	Player();
	~Player();

	bool Awake(pugi::xml_node& config);
	bool Start();
	bool PreUpdate();
	bool Update(float dt);
	bool PostUpdate();
	bool CleanUp();
	bool LoadState(pugi::xml_node&);
	bool SaveState(pugi::xml_node&) const;
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
	int level;
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
	SString spritesPath;
	Animation* currentAnim;
	Animation idle, run, jump, death;
	float animSpeed;

	Timer shotCoolDown;

};

#endif