#ifndef __PLAYER_H
#define __PLAYER_H

#pragma once
#include "Module.h"
#include "Collider.h"

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

	bool isStuck = false;
	bool grounded;
	int jumpForce;
	float gravity;
	int jumps;

};

#endif