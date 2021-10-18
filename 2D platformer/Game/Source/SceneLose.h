#ifndef __SCENE_LOSE__
#define __SCENE_LOSE__

#include "Module.h"

struct SDL_Texture;

class SceneLose : public Module
{
public:

	SceneLose();
	virtual ~SceneLose();

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