#include "App.h"
#include "Window.h"
#include "Input.h"
#include "Render.h"
#include "Textures.h"
#include "Audio.h"
#include "SceneStart.h"
#include "SceneGame.h"
#include "SceneWin.h"
#include "SceneLose.h"
#include "FadeToBlack.h"
#include "Map.h"
#include "PathFinding.h"
#include "Collisions.h"
#include "Player.h"
#include "EntityManager.h"
#include "Checkpoint.h"
#include "Fonts.h"
#include "GuiManager.h"

#include "Defs.h"
#include "Log.h"

#include <iostream>
#include <sstream>

// Constructor
App::App(int argc, char* args[]) : argc(argc), args(args)
{
	frames = 0;

	win = new Window();
	input = new Input();
	render = new Render();
	tex = new Textures();
	audio = new Audio();
	sceneStart = new SceneStart();
	sceneGame = new SceneGame();
	sceneWin = new SceneWin();
	sceneLose = new SceneLose();
	fade = new FadeToBlack();
	map1 = new Map();
	map2 = new Map();
	pathfinding = new PathFinding();
	collisions = new Collisions();
	entityManager = new EntityManager();
	fonts = new Fonts();
	guiManager = new GuiManager();
	//checkpoint = new Checkpoint();
	//player = new Player;

	// Ordered for awake / Start / Update
	// Reverse order of CleanUp
	AddModule(win);
	AddModule(input);
	AddModule(tex);
	AddModule(audio);
	AddModule(fonts);

	AddModule(map1);
	AddModule(map2);
	AddModule(entityManager);

	AddModule(sceneStart);
	AddModule(sceneGame);
	AddModule(sceneWin);
	AddModule(sceneLose);
	//AddModule(checkpoint);
	AddModule(collisions);
	
	AddModule(guiManager);

	AddModule(fade);

	// Render last to swap buffer
	AddModule(render);
}

// Destructor
App::~App()
{
	// Release modules
	ListItem<Module*>* item = modules.end;

	while(item != NULL)
	{
		RELEASE(item->data);
		item = item->prev;
	}

	modules.clear();
}

void App::AddModule(Module* module)
{
	module->Init();
	modules.add(module);
}

// Called before render is available
bool App::Awake()
{
	
	pugi::xml_node configApp;

	bool ret = false;

	// L01: DONE 3: Load config from XML
	config = LoadConfig(configFile);
	
	map1->name.Create("map1");
	map2->name.Create("map2");

	if (config.empty() == false)
	{
		ret = true;
		configApp = config.child("app");

		// L01: DONE 4: Read the title from the config file
		title.Create(configApp.child("title").child_value());
		organization.Create(configApp.child("organization").child_value());
	}

	if (ret == true)
	{
		ListItem<Module*>* item;
		item = modules.start;

		while ((item != NULL) && (ret == true))
		{
			// L01: DONE 5: Add a new argument to the Awake method to receive a pointer to an xml node.
			// If the section with the module name exists in config.xml, fill the pointer with the valid xml_node
			// that can be used to read all variables for that module.
			// Send nullptr if the node does not exist in config.xml
			ret = item->data->Awake(config.child(item->data->name.GetString()));
			item = item->next;
		}
	}

	/*entityManager->AddEntity(player);
	player->Awake(config.child("player"));
	player->Start();*/

	return ret;
}

// Called before the first frame
bool App::Start()
{
	bool ret = true;
	ListItem<Module*>* item;
	item = modules.start;

	while(item != NULL && ret == true)
	{
		ret = item->data->Start();
		item = item->next;
	}
	//player->Start();
	return ret;
}

// Called each loop iteration
bool App::Update()
{
	bool ret = true;
	PrepareUpdate();

	if(input->GetWindowEvent(WE_QUIT) == true)
		ret = false;

	if(ret == true)
		ret = PreUpdate();

	if(ret == true)
		ret = DoUpdate();

	if(ret == true)
		ret = PostUpdate();

	if (exit == true) ret = false;

	FinishUpdate();
	return ret;
}

// Load config from XML file
// NOTE: Function has been redesigned to avoid storing additional variables on the class
pugi::xml_node App::LoadConfig(pugi::xml_document& configFile) const
{
	pugi::xml_node ret;

	pugi::xml_parse_result result = configFile.load_file(CONFIG_FILENAME);

	if (result == NULL) LOG("Could not load xml file: %s. pugi error: %s", CONFIG_FILENAME, result.description());
	else ret = configFile.child("config");

	return ret;
}

// ---------------------------------------------
void App::PrepareUpdate()
{
}

// ---------------------------------------------
void App::FinishUpdate()
{
	float fps = 0.0f;
	// L02: DONE 1: This is a good place to call Load / Save methods
	if (loadGameRequested == true) LoadGame();
	if (saveGameRequested == true) SaveGame();

	// frame control

	frameCount++;
	if (cap30fps==true)
		while (lastFrameTime.Read() < (1000 / 30)) {}
	else if (app->render->vsync == true)
		while (lastFrameTime.Read() < (1000 / 60)) {}

	dt = lastFrameTime.Read();
	lastFrameTime.Start();

	fps = 1000 / dt;
	float averageFps = float(frameCount) / startupTime.ReadSec();
	uint32 lastFrameMs = frameTime.Read();

	static char title[256];

	if (app->render->vsync == true)
		sprintf_s(title, 256, "FPS: %.2f  Av.FPS: %.2f  Last Frame Ms: %02u  Vsync: on",
			fps, averageFps, lastFrameMs);
	else
		sprintf_s(title, 256, "FPS: %.2f  Av.FPS: %.2f  Last Frame Ms: %02u  Vsync: off",
			fps, averageFps, lastFrameMs);

	SString newTitle = title;

	app->win->SetTitle(newTitle.GetString());

	
}

// Call modules before each loop iteration
bool App::PreUpdate()
{
	bool ret = true;

	ListItem<Module*>* item;
	Module* pModule = NULL;

	for(item = modules.start; item != NULL && ret == true; item = item->next)
	{
		pModule = item->data;

		if(pModule->active == false) {
			continue;
		}

		ret = item->data->PreUpdate();
	}

	return ret;
}

// Call modules on each loop iteration
bool App::DoUpdate()
{
	bool ret = true;
	ListItem<Module*>* item;
	item = modules.start;
	Module* pModule = NULL;

	for(item = modules.start; item != NULL && ret == true; item = item->next)
	{
		pModule = item->data;

		if(pModule->active == false) {
			continue;
		}

		ret = item->data->Update(dt);
	}

	return ret;
}

// Call modules after each loop iteration
bool App::PostUpdate()
{
	bool ret = true;
	ListItem<Module*>* item;
	Module* pModule = NULL;

	for(item = modules.start; item != NULL && ret == true; item = item->next)
	{
		pModule = item->data;

		if(pModule->active == false) {
			continue;
		}

		ret = item->data->PostUpdate();
	}

	return ret;
}

// Called before quitting
bool App::CleanUp()
{
	bool ret = true;
	ListItem<Module*>* item;
	item = modules.end;

	while(item != NULL && ret == true)
	{
		ret = item->data->CleanUp();
		item = item->prev;
	}

	return ret;
}

// ---------------------------------------
int App::GetArgc() const
{
	return argc;
}

// ---------------------------------------
const char* App::GetArgv(int index) const
{
	if(index < argc)
		return args[index];
	else
		return NULL;
}

// ---------------------------------------
const char* App::GetTitle() const
{
	return title.GetString();
}

// ---------------------------------------
const char* App::GetOrganization() const
{
	return organization.GetString();
}

// Load / Save
void App::LoadGameRequest()
{
	if (gameStateFile == NULL) loadGameRequested = false;
	else loadGameRequested = true;
}

// ---------------------------------------
void App::SaveGameRequest() const
{
	
	saveGameRequested = true;
}

// ---------------------------------------
// L02: TODO 5: Create a method to actually load an xml file
// then call all the modules to load themselves
bool App::LoadGame()
{
	bool ret = true;
	pugi::xml_parse_result result = gameStateFile.load_file("savegame.xml");

	if (result == NULL)
	{
		LOG("Could not load map xml file config.xml. pugi error: %s", result.description());
		ret = false;
	}

	ListItem<Module*>* item;
	item = modules.start;

	while (item != NULL && ret == true)
	{
		//pugi::xml_node moduleNode = saveStateNode.child(item->data->name.GetString());
		ret = item->data->LoadState(gameStateFile.child("game_state").child(item->data->name.GetString()));
		//ret = item->data->LoadState(moduleNode);
		item = item->next;
	}
	sceneGame->ChangeMap();

	loadGameRequested = false;

	return ret;
}

// L02: TODO 7: Implement the xml save method for current state
bool App::SaveGame() const
{

	bool ret = false;
	pugi::xml_document* saveDoc = new pugi::xml_document();
	pugi::xml_node saveStateNode = saveDoc->append_child("game_state");

	ListItem<Module*>* item;
	item = modules.start;

	while (item != NULL)
	{
		//pugi::xml_node moduleNode = saveStateNode.child(item->data->name.GetString());
		ret = item->data->SaveState(saveStateNode.append_child(item->data->name.GetString()));
		item = item->next;
	}
	ret = saveDoc->save_file("savegame.xml");

	sceneGame->saved = true;

	saveGameRequested = false;

	return ret;
}

bool App::GetLoadGameRequested()
{
	return loadGameRequested;
}