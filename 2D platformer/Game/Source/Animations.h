#ifndef  __ANIMATIONS_H__
#define __ANIMATIONS_H__

#pragma once

#include "Module.h"

#include "List.h"
#include "SDL_image/include/SDL_image.h"

struct Anim
{
	Anim(SDL_Texture* texture, int duratioSprite)
	{
		spriteSheet = texture;
	}
	~Anim()
	{
		sprites.clear();
	}
	
	void AddSprite(SDL_Rect* rect)
	{
		sprites.add(rect);
		spritesNum++;
	}
	
	SDL_Texture* spriteSheet;
	List<SDL_Rect*> sprites;
	int spriteFrames;
	int spritesNum;
	int curFrame;
};


class Animations : public Module
{
public:
	Animations();
	virtual ~Animations();

	bool Awake();
	bool Start();
	bool PreUpdate();
	bool Update(float dt);
	bool PostUpdate();
	bool CleanUp();

private:
	SDL_Texture* img;
};

#endif