#ifndef __APP_H__
#define __APP_H__

#include "Module.h"
#include "List.h"

#include "Timer.h"
#include "PerfTimer.h"

#include "PugiXml/src/pugixml.hpp"

#define CONFIG_FILENAME		"config.xml"
#define SAVE_STATE_FILENAME "save_game.xml"

// Modules
class Window;
class Input;
class Render;
class Textures;
class Audio;
class SceneStart;
class SceneGame;
class SceneWin;
class SceneLose;
class SceneManager;
class FadeToBlack;
class Map;
class Collisions;
class Physics;
class Player;

class App
{
public:

	// Constructor
	App(int argc, char* args[]);

	// Destructor
	virtual ~App();

	// Called before render is available
	bool Awake();

	// Called before the first frame
	bool Start();

	// Called each loop iteration
	bool Update();

	// Called before quitting
	bool CleanUp();

	// Add a new module to handle
	void AddModule(Module* module);

	// Exposing some properties for reading
	int GetArgc() const;
	const char* GetArgv(int index) const;
	const char* GetTitle() const;
	const char* GetOrganization() const;

    // L02: DONE 1: Create methods to request Load / Save
	void LoadGameRequest();
	void SaveGameRequest() const;
	bool GetLoadGameRequested();

private:

	pugi::xml_node LoadConfig(pugi::xml_document&) const;

	void PrepareUpdate();
	void FinishUpdate();
	bool PreUpdate();
	bool DoUpdate();
	bool PostUpdate();

	// Load / Save
	bool LoadGame();
	bool SaveGame() const;

public:

	// Modules
	Window* win;
	Input* input;
	Render* render;
	Textures* tex;
	Audio* audio;
	SceneStart* sceneStart;
	SceneGame* sceneGame;
	SceneWin* sceneWin;
	SceneLose* sceneLose;
	FadeToBlack* fade;
	Map* map1;
	Map* map2;
	Collisions* collisions;
	Physics* physics;
	Player* player;

	float dt = 0.0f;


private:

	int argc;
	char** args;
	SString title;
	SString organization;

	List<Module *> modules;

	pugi::xml_document gameStateFile;

	uint frames;
	mutable bool saveGameRequested;
	bool loadGameRequested;

	PerfTimer ptimer;
	uint64 frameCount = 0;

	Timer startupTime;
	Timer frameTime;
	Timer lastFrameTime;
	uint32 lastSecFrameCount = 0;
	uint32 prevLastSecFrameCount = 0;
};

extern App* app;

#endif	// __APP_H__