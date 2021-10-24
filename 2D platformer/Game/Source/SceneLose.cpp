#include "SceneLose.h"

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

SceneLose::SceneLose() :Module()
{
	name.Create("lose");
	LOG("LOSE");
}

SceneLose::~SceneLose()
{}

bool SceneLose::Awake()
{
	return true;
}

bool SceneLose::Start()
{
	active = false;
	return true;
}

bool SceneLose::PreUpdate()
{
	return true;
}

bool SceneLose::Update(float dt)
{

	SString title("LOSE");
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