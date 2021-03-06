#include "SceneWin.h"
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

SceneWin::SceneWin() :Module()
{
	name.Create("win");
	LOG("Win");
}

SceneWin::~SceneWin()
{}

bool SceneWin::Awake(pugi::xml_node& config)
{
	imgPath = config.attribute("img").as_string();
	LOG("Loading Scene");
	bool ret = true;

	return ret;
}

bool SceneWin::Start()
{
	img = app->tex->Load(imgPath.GetString());
	active = false;

	return true;
}

bool SceneWin::PreUpdate()
{
	return true;
}

bool SceneWin::Update(float dt)
{
	app->player->active = false;
	app->render->camera.x = 0;
	app->render->camera.y = 0;

	app->render->DrawTexture(img, 0, 0, NULL);


	if (app->input->GetKey(SDL_SCANCODE_F1) == KEY_DOWN)
	{
		app->sceneGame->level = 1;
		app->fade->Fade(this, app->sceneGame, app->fade->time / dt);
	}

	if (app->input->GetKey(SDL_SCANCODE_F2) == KEY_DOWN)
	{
		app->sceneGame->level = 2;
		app->fade->Fade(this, app->sceneGame, app->fade->time / dt);
	}

	if (app->input->GetKey(SDL_SCANCODE_RETURN) == KEY_DOWN)
		app->fade->Fade(this, app->sceneStart, app->fade->time / dt);

	if (app->input->GetKey(SDL_SCANCODE_F5) == KEY_DOWN) app->LoadGameRequest();

	if (app->input->GetKey(SDL_SCANCODE_F11) == KEY_DOWN) app->cap30fps = !app->cap30fps;


	SString title("Win");
	//app->win->SetTitle(title.GetString());
	return true;
}

bool SceneWin::PostUpdate()
{
	bool ret = true;

	if (app->input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN)
		ret = false;

	return ret;
}

bool SceneWin::CleanUp()
{
	return true;
}

