#include "App.h"
#include "Input.h"
#include "SceneTitle.h"
#include "Audio.h"
#include "Render.h"
#include "Window.h"
#include "SceneLevel1.h"
#include "Map.h"
#include "FadeToBlack.h"

#include "Defs.h"
#include "Log.h"

SceneTitle::SceneTitle() : Module()
{
	name.Create("sceneTitle");
	active = true;
}

// Destructor
SceneTitle::~SceneTitle()
{}

// Called before render is available
bool SceneTitle::Awake()
{
	LOG("Loading Scene");
	bool ret = true;

	return ret;
}

// Called before the first frame
bool SceneTitle::Start()
{
	// L03: DONE: Load map
	app->map->Draw();
	app->map->Load("hello.tmx");
	
	
	
	return true;
}

// Called each loop iteration
bool SceneTitle::PreUpdate()
{
	return true;
}

// Called each loop iteration
bool SceneTitle::Update(float dt)
{
	// Start level 1
	if (app->input->GetKey(SDL_SCANCODE_RETURN) == KEY_DOWN)
		app->fade->Fade(app->sceneTitle, app->sceneLevel1, 5000); // in miliseconds

	
	//app->render->DrawTexture(img, 380, 100); // Placeholder not needed any more

	// Draw map
	app->map->Draw();
	app->render->DrawTexture(img, 380, 100);

	return true;
}

// Called each loop iteration
bool SceneTitle::PostUpdate()
{
	bool ret = true;

	if (app->input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN)
		ret = false;

	return ret;
}

// Called before quitting
bool SceneTitle::CleanUp()
{
	LOG("Freeing scene");

	return true;
}
