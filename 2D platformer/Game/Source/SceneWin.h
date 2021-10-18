#ifndef __SCENE_WIN__
#define __SCENE_WIN__

#include "Module.h"

struct SDL_Texture;

class SceneWin : public Module
{
public:

	SceneWin();
	virtual ~SceneWin();

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