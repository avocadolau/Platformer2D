#include "App.h"
#include "Input.h"
#include "SceneTitle.h"
#include "Audio.h"
#include "Render.h"
#include "Window.h"
#include "SceneLevel1.h"
#include "Map.h"
#include "FadeToBlack.h"
#include "Player.h"

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
	/*app->map->Draw();
	app->map->Load("hello.tmx");
	*/
	
	
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
	if ((app->input->GetKey(SDL_SCANCODE_RETURN) == KEY_DOWN)
		|| (app->input->GetKey(SDL_SCANCODE_F1) == KEY_DOWN))
	{
		app->fade->Fade(app->sceneTitle, app->sceneLevel1, FADE_TIME);
		app->player->active = true;
		app->audio->PlayMusic("Assets/audio/music/music_spy.ogg");
	}

	



	SString title("Tittle");
	app->win->SetTitle(title.GetString());

	// Draw map -- se tiene q cambiar al fondo bonito q hara sofia hehe
	/*app->map->Draw();
	app->render->DrawTexture(img, 380, 100);*/

	return true;
}

// Called each loop iteration
bool SceneTitle::PostUpdate()
{
	bool ret = true;

	/*if (app->input->GetKey(SDL_SCANCODE_F2) == KEY_DOWN)
	{
		app->fade->Fade(app->sceneTitle, app->sceneLevel2, FADE_TIME);
		app->player->active = true;
		app->audio->PlayMusic("Assets/audio/music/music_spy.ogg");
	}*/

	if (app->input->GetKey(SDL_SCANCODE_F5) == KEY_DOWN)
	{
		// save game
	}

	if (app->input->GetKey(SDL_SCANCODE_F6) == KEY_DOWN)
	{
		// load the previous state
	}

	if (app->input->GetKey(SDL_SCANCODE_F9) == KEY_DOWN)
	{
		// load the previous state
	}

	if (app->input->GetKey(SDL_SCANCODE_F6) == KEY_DOWN)
	{
		// god mode pero eso hay q hacerlo en el player xD
	}


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
