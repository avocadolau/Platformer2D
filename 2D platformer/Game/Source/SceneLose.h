#ifndef __SCENE_LOSE_H__
#define __SCENE_LOSE_H__

#include "Module.h"

struct SDL_Texture;

class SceneLose : public Module
{
public:

	SceneLose();
	virtual ~SceneLose();

	bool Awake(pugi::xml_node& config);
	bool Start();
	bool PreUpdate();
	bool Update(float dt);
	bool PostUpdate();
	bool CleanUp();

private:
	SDL_Texture* img;
	SString imgPath;
	bool activeLastFrame;
};

#endif