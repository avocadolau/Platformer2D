#ifndef __SCENE_H__
#define __SCENE_H__

#include "Module.h"

struct SDL_Texture;

class SceneLevel1 : public Module
{
public:

	SceneLevel1();

	// Destructor
	virtual ~SceneLevel1();

	// Called before render is available
	bool Awake();

	// Called before the first frame
	bool Start();

	// Called before all Updates
	bool PreUpdate();

	// Called each loop iteration
	bool Update(float dt);

	// Called before all Updates
	bool PostUpdate();

	// Called before quitting
	bool CleanUp();

private:
	SDL_Texture* img;
};

#endif // __SCENE_H__