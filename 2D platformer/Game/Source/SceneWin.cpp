#include "SceneWin.h"
#include "App.h"
#include "Input.h"
#include "Textures.h"
#include "Audio.h"
#include "Render.h"
#include "Window.h"
#include "SceneLevel1.h"
#include "Map.h"
#include "Player.h"

#include "Defs.h"
#include "Log.h"

SceneWin::SceneWin() :Module()
{
	name.Create("SceneWin");
	LOG("Win");
}

SceneWin::~SceneWin()
{}

bool SceneWin::Awake()
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

