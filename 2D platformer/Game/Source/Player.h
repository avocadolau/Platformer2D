#ifndef __PLAYER_H
#define __PLAYER_H

#pragma once
#include "Module.h"
#include "Collider.h"
#include "Animation.h"
#include "Textures.h"

enum State
{
	IDLE,
	RUN,
	JUMP,
	DEATH
};

class Player :
    public Module
{
public:
	
	Player();
	~Player();

	bool Awake();
	bool Start();
	bool PreUpdate();
	bool Update(float dt);
	bool PostUpdate();
	bool CleanUp();
	bool LoadState(pugi::xml_node&);
	bool SaveState(pugi::xml_node&);
	void OnCollision(Collider* c1, Collider* c2);

public:

	fPoint pos;
	fPoint camPos;
	fPoint vel;
	fPoint maxVel;
	Collider* collider;
	//PhysBody* body;

	State state;

	Collider* colUp;
	Collider* colDown;
	Collider* colRight;
	Collider* colLeft;
	bool up = true;
	bool down = true;
	bool left = true;
	bool right = true;

	float gravity;
	int jumps;

	int level;

	iPoint dim = { 35,64 };


private:
	SDL_Texture* sprites;
	Animation idle, run, jump, death;
	Animation* currentAnim;
};

#endif