#include "Log.h"

#include "App.h"
#include "Map.h"
#include "Render.h"
#include "SceneElements.h"
#include "PathFinding.h"
#include "DynArray.h"
#include "Point.h"
#include "Player.h"
#include "Render.h"

#include "SDL/include/SDL_Rect.h"
SceneElements::SceneElements() :Module()
{
	name.Create("elements");

	
}

SceneElements::~SceneElements()
{


}
bool SceneElements::Awake(pugi::xml_node& config)
{
	LOG("Loading Scene Elements");
	bool ret = true;

	platformPath = config.child("platform").attribute("platformPath").as_string();

	return ret;
}


bool SceneElements::Start()
{

	platformImg = app->tex->Load(platformPath.GetString());
	return true;
}

bool SceneElements::PreUpdate()
{



	return true;
}

bool SceneElements::Update(float dt)
{
	ListItem<Platform*>* item = platforms.start;
	while (item != NULL)
	{
		item->data->Update(dt);
		app->render->DrawTexture(platformImg, item->data->pos.x, item->data->pos.y, NULL);

		item = item->next;
	}

	return true;
}

bool SceneElements::PostUpdate()
{
	
	if (app->debug == true) DrawPaths();

	return true;
}

bool SceneElements::CleanUp()
{

	platforms.clear();

	return true;
}

void SceneElements::DrawPaths()
{
	
}