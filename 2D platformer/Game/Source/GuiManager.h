#ifndef __GUIMANAGER_H__
#define __GUIMANAGER_H__

#include "Module.h"
#include "GuiControl.h"

#include "List.h"

class GuiManager : public Module
{
public:

	// Constructor
	GuiManager();

	// Destructor
	virtual ~GuiManager();

	// Called before the first frame
	 bool Start();

	 bool Update(float dt);
	 bool PostUpdate();

	bool UpdateAll(float dt,bool logic);

	bool Draw();

	// Called before quitting
	bool CleanUp();

	// Additional methods
	GuiControl* CreateGuiControl(GuiControlType type, int id, const char* text, SDL_Rect bounds, Module* observer, SDL_Rect sliderBounds = { 0,0,0,0 });
	void DestroyGuiControl(GuiControl* entity);
	void AddGuiControl(GuiControl* entity);

	bool OnGuiMouseClickEvent(GuiControl* control);

public:

	int fontColor = 0;

	List<GuiControl*> controls;

	List<GuiControl*> mainMenu;
	List<GuiControl*> pauseMenu;
	List<GuiControl*> settingsMenu;


	bool hud = false;
	bool mainMenuActive = false;
	bool pauseMenuActive = false;
	bool settingsMenuActive = false;

	float accumulatedTime = 0.0f;
	float updateMsCycle = 0.0f;
	bool doLogic = false;

	SDL_Texture* texture;
	SDL_Texture* heart;
	SDL_Texture* hudImg;
	SDL_Texture* mainMenuTitle;

};

#endif // __GUIMANAGER_H__
