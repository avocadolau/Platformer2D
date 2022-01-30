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
#include "GuiManager.h"
#include "GuiControl.h"

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
	app->sceneGame->level = 1;
	
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

	if (credits == true)
	{
		SDL_Rect rec = { 0,0,1280,720 };
		app->render->DrawRectangle(rec, 0, 0, 0, 255);
		if (app->input->GetKey(SDL_SCANCODE_RETURN) == KEY_DOWN) credits = false;
	}

	else
	{
		// Start level 1
		if ((app->input->GetKey(SDL_SCANCODE_RETURN) == KEY_DOWN)
			|| (app->input->GetKey(SDL_SCANCODE_F1) == KEY_DOWN) || lvl1 == true)
		{
			lvl1 = false;
			app->sceneGame->level = 1;
			app->fade->Fade(this, app->sceneGame, app->fade->time / dt);
		}

		if (app->input->GetKey(SDL_SCANCODE_F2) == KEY_DOWN)
		{
			app->sceneGame->level = 2;
			app->fade->Fade(this, app->sceneGame, app->fade->time / dt);
		}

		if (app->input->GetKey(SDL_SCANCODE_F5) == KEY_DOWN) app->LoadGameRequest();

		if (app->input->GetKey(SDL_SCANCODE_F11) == KEY_DOWN) app->cap30fps = !app->cap30fps;
	}
	


	SString Start("I see the light");
	//app->win->SetTitle(Start.GetString());

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
	{
		if (credits == true) credits = false;
		else ret = false;
	}

	return ret;
}

// Called before quitting
bool SceneStart::CleanUp()
{
	LOG("Freeing scene");

	return true;
}


bool SceneStart::OnGuiMouseClickEvent(GuiControl* control)
{

	switch (control->id)
	{
	case 1:		lvl1 = true;				break;
	case 2:		app->LoadGameRequest();		break;
	case 4:		credits = true;				break;
	case 5:		app->exit = true;			break;
	
	}

	return true;
}
