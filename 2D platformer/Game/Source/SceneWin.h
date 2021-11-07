#ifndef __SCENE_WIN_H__
#define __SCENE_WIN_H__

#include "Module.h"

struct SDL_Texture;

class SceneWin : public Module
{
public:

	SceneWin();
	virtual ~SceneWin();

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