#ifndef __SCENE_TITLE_H__
#define __SCENE_TITLE_H__

#include "Module.h"

struct SDL_Texture;

class SceneStart : public Module
{
public:

	SceneStart();

	// Destructor
	virtual ~SceneStart();

	// Called before render is available
	bool Awake(pugi::xml_node& config);

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

	bool OnGuiMouseClickEvent(GuiControl* control);

public:
	bool credits;
private:
	SDL_Texture* img;
	SString imgPath;

	bool lvl1, lvl2;
};

#endif // __SCENE_H__
