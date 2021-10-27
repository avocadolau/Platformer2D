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
	Collider* feet;
	Collider* lastCol;
	//PhysBody* body;

	State state;

	Collider* colUp;
	Collider* colDown;
	Collider* right;
	Collider* left;
	bool detected[4];

	bool isStuck = false;
	bool grounded;
	float gravity;
	int jumps;


private:
	SDL_Texture* sprites;
	iPoint dimension = { 64,64 };
	Animation idle, run, jump, death;
	Animation* currentAnim;
};

#endif