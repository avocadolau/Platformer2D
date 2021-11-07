#include "SceneLose.h"
#include "App.h"
#include "Input.h"
#include "Textures.h"
#include "Audio.h"
#include "Render.h"
#include "Window.h"
#include "SceneStart.h"
#include "SceneGame.h"
#include "FadeToBlack.h"
#include "Map.h"
#include "Player.h"

#include "Defs.h"
#include "Log.h"

SceneLose::SceneLose() :Module()
{
	name.Create("lose");
	LOG("Lose");
}

SceneLose::~SceneLose()
{}

bool SceneLose::Awake(pugi::xml_node& config)
{
	imgPath = config.attribute("img").as_string();
	LOG("Loading Scene");
	bool ret = true;

	return ret;
}

bool SceneLose::Start()
{
	img = app->tex->Load(imgPath.GetString());
	active = false;

	return true;
}

bool SceneLose::PreUpdate()
{
	if (activeLastFrame == false)
		if (active == true)
			app->audio->PlayFx(3);
	return true;
}

bool SceneLose::Update(float dt)
{
	if (active == true) activeLastFrame = true;
	else activeLastFrame = false;

	app->player->active = false;
	app->render->camera.x = 0;
	app->render->camera.y = 0;

	app->render->DrawTexture(img, 0, 0, NULL);
	app->render->DrawTexture(app->player->sprites, app->win->GetWidth() / 2, app->win->GetHeight() / 2, &app->player->death.GetCurrentFrame(), NULL);

	if (app->input->GetKey(SDL_SCANCODE_F1) == KEY_DOWN)
	{
		
		app->player->level = 1;
		app->fade->Fade(this, app->sceneGame, app->fade->time / dt);
		app->player->death.Reset();
	}

	if (app->input->GetKey(SDL_SCANCODE_F2) == KEY_DOWN)
	{
		app->player->level = 2;
		app->fade->Fade(this, app->sceneGame, app->fade->time / dt);
	}

	if (app->input->GetKey(SDL_SCANCODE_RETURN) == KEY_DOWN)
		app->fade->Fade(this, app->sceneStart, app->fade->time / dt);

	if (app->input->GetKey(SDL_SCANCODE_F5) == KEY_DOWN) app->LoadGameRequest();

	SString title("Lose");
	app->win->SetTitle(title.GetString());
	return true;
}

bool SceneLose::PostUpdate()
{
	bool ret = true;

	if (app->input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN)
		ret = false;

	return ret;
}

bool SceneLose::CleanUp()
{
	return true;
}

