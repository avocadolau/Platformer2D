#ifndef __SCENE_LEVEL1__
#define __SCENE_LEVEL1__

#include "Module.h"

struct SDL_Texture;

class SceneLevel1 : public Module
{
public:

	SceneLevel1();
	virtual ~SceneLevel1();

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