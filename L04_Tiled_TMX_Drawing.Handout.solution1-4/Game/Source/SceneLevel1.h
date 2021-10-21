#ifndef __SCENE_LEVEL1_H__
#define __SCENE_LEVEL1_H__

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

	bool LoadState(pugi::xml_node&);
	bool SaveState(pugi::xml_node&) const;

private:
	SDL_Texture* img;
};

#endif // __SCENE_H__