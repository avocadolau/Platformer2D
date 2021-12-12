#ifndef __CHECKPOINT
#define __CHECKPOINT

#pragma once
#include "Module.h"
#include "Collider.h"
#include "Animation.h"
#include "Textures.h"

class Checkpoint : public Module
{
public:
	
	Checkpoint();
	~Checkpoint();

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
	Collider* collider;
	SDL_Rect dim;

	SDL_Texture* sprites;
	SString spritesPath;

	bool activated = false;
};

#endif