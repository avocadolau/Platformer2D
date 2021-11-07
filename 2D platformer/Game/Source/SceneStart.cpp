#include "App.h"
#include "Input.h"
#include "SceneStart.h"
#include "Audio.h"
#include "Render.h"
#include "Window.h"
#include "SceneGame.h"
#include "Map.h"
#include "FadeToBlack.h"
#include "Player.h"

#include "Defs.h"
#include "Log.h"

SceneStart::SceneStart() : Module()
{
	name.Create("start");
	active = true;
}

// Destructor
SceneStart::~SceneStart()
{}

// Called before render is available
bool SceneStart::Awake(pugi::xml_node& config)
{
	imgPath = config.attribute("img").as_string();
	LOG("Loading Scene");
	bool ret = true;

	return ret;
}

// Called before the first frame
bool SceneStart::Start()
{
	img= app->tex->Load(imgPath.GetString());
	app->audio->PlayMusic(app->audio->soundtrack.GetString());
	app->player->level = 1;
	
	return true;
}

// Called each loop iteration
bool SceneStart::PreUpdate()
{
	return true;
}

// Called each loop iteration
bool SceneStart::Update(float dt)
{
	app->player->active = false;
	app->render->camera.x = 0;
	app->render->camera.y = 0;

	app->render->DrawTexture(img, 0, 0, NULL);

	// Start level 1
	if ((app->input->GetKey(SDL_SCANCODE_RETURN) == KEY_DOWN)
		|| (app->input->GetKey(SDL_SCANCODE_F1) == KEY_DOWN))
	{
		app->player->level = 1;
		app->fade->Fade(this, app->sceneGame, app->fade->time / dt);
		//app->audio->PlayMusic("Assets/audio/music/music_spy.ogg");
	}

	/*if (app->input->GetKey(SDL_SCANCODE_F2) == KEY_DOWN)
	{
		app->player->level = 2;
		app->fade->Fade(this, app->sceneLevel1, app->fade->time / dt);
		app->sceneLevel1->ChangeMap();
	}*/

	if (app->input->GetKey(SDL_SCANCODE_F5) == KEY_DOWN) app->LoadGameRequest();

	SString Start("Tittle");
	app->win->SetTitle(Start.GetString());

	// Draw map -- se tiene q cambiar al fondo bonito q hara sofia hehe
	/*app->map->Draw();
	app->render->DrawTexture(img, 380, 100);*/

	return true;
}

// Called each loop iteration
bool SceneStart::PostUpdate()
{
	bool ret = true;

	/*if (app->input->GetKey(SDL_SCANCODE_F2) == KEY_DOWN)
	{
		app->fade->Fade(app->sceneStart, app->sceneLevel2, FADE_TIME);
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
bool SceneStart::CleanUp()
{
	LOG("Freeing scene");

	return true;
}
