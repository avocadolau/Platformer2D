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
	LOG("Loading Scene");
	bool ret = true;

	return ret;
}

bool SceneWin::Start()
{
	// L03: DONE: Load background
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

	app->render->DrawTexture(app->player->sprites, app->win->GetWidth() / 2, app->win->GetHeight() / 2, &app->player->idle.GetCurrentFrame(), NULL);


	if (app->input->GetKey(SDL_SCANCODE_F1) == KEY_DOWN)
	{
		app->player->level = 1;
		app->fade->Fade(this, app->sceneGame, app->fade->time / dt);
		//app->audio->PlayMusic("Assets/audio/music/music_spy.ogg");
	}

	if (app->input->GetKey(SDL_SCANCODE_RETURN) == KEY_DOWN)
		app->fade->Fade(this, app->sceneStart, app->fade->time / dt);

	if (app->input->GetKey(SDL_SCANCODE_F5) == KEY_DOWN) app->LoadGameRequest();

	SString title("Win");
	app->win->SetTitle(title.GetString());
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

