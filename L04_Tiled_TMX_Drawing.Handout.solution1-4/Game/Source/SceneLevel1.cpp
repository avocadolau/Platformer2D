#include "App.h"
#include "Input.h"
#include "Textures.h"
#include "Audio.h"
#include "Render.h"
#include "Window.h"
#include "SceneLevel1.h"
#include "FadeToBlack.h"
#include "Player.h"
#include "Map.h"

#include "Defs.h"
#include "Log.h"

SceneLevel1::SceneLevel1() : Module()
{
	name.Create("sceneLevel1");
	LOG("Level 1");
}

// Destructor
SceneLevel1::~SceneLevel1()
{}

// Called before render is available
bool SceneLevel1::Awake()
{
	LOG("Loading Scene");
	bool ret = true;

	return ret;
}

// Called before the first frame
bool SceneLevel1::Start()
{
	// L03: DONE: Load map
	active = false;
	app->map->Load("hello.tmx");
	app->audio->PlayMusic("Assets/audio/music/music_spy.ogg");

	return true;
}

// Called each loop iteration
bool SceneLevel1::PreUpdate()
{
	return true;
}

// Called each loop iteration
bool SceneLevel1::Update(float dt)
{
    // L02: DONE 3: Request Load / Save when pressing L/S
	if(app->input->GetKey(SDL_SCANCODE_L) == KEY_DOWN)
		app->LoadGameRequest();

	if(app->input->GetKey(SDL_SCANCODE_S) == KEY_DOWN)
		app->SaveGameRequest();

	/*if(app->input->GetKey(SDL_SCANCODE_UP) == KEY_REPEAT)
		app->render->camera.y -= 1;

	if(app->input->GetKey(SDL_SCANCODE_DOWN) == KEY_REPEAT)
		app->render->camera.y += 1;

	if(app->input->GetKey(SDL_SCANCODE_LEFT) == KEY_REPEAT)
		app->render->camera.x -= 1;

	if(app->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_REPEAT)
		app->render->camera.x += 1;*/


	// Draw map
	app->map->Draw();

	// L03: DONE 7: Set the window title with map/tileset info
	/*SString title("Map:%dx%d Tiles:%dx%d Tilesets:%d",
				   app->map->mapData.width, app->map->mapData.height,
				   app->map->mapData.tileWidth, app->map->mapData.tileHeight,
				   app->map->mapData.tilesets.count());*/

	SString title("Level 1");
	app->win->SetTitle(title.GetString());

	return true;
}

// Called each loop iteration
bool SceneLevel1::PostUpdate()
{
	bool ret = true;

	if(app->input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN)
		ret = false;

	return ret;
}

bool SceneLevel1::LoadState(pugi::xml_node&)
{
	return true;
}

bool SceneLevel1::SaveState(pugi::xml_node&) const
{
	return true;
}

// Called before quitting
bool SceneLevel1::CleanUp()
{
	LOG("Freeing scene");

	return true;
}
