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

public:
	int jumpSpeed;

};

#endif