#include "App.h"
#include "Input.h"
#include "Textures.h"
#include "Audio.h"
#include "Render.h"
#include "Window.h"
#include "SceneGame.h"
#include "Collisions.h"
#include "Collider.h"
#include "Map.h"
#include "Player.h"

#include "Defs.h"
#include "Log.h"

SceneGame::SceneGame() : Module()
{
	name.Create("game");
}

// Destructor
SceneGame::~SceneGame()
{}

// Called before render is available
bool SceneGame::Awake(pugi::xml_node& config)
{
	LOG("Loading Scene");
	bool ret = true;
	
	platformPath = config.attribute("platformPath").as_string();
	backgroundPath = config.attribute("backgroundPath").as_string();
	parallax = config.attribute("parallax").as_float();

	return ret;
}

// Called before the first frame
bool SceneGame::Start()
{
	active = false;
	app->map1->Load("level1.tmx");
	//app->map2->Load("level2.tmx");
	app->player->active = true;
	platformImg = app->tex->Load(platformPath.GetString());
	background = app->tex->Load(backgroundPath.GetString());

	// Load music
	//app->audio->PlayMusic("Assets/audio/music/music_spy.ogg");

	return true;
}

// Called each loop iteration
bool SceneGame::PreUpdate()
{
	app->player->active = true;
	return true;
}

// Called each loop iteration
bool SceneGame::Update(float dt)
{

	iPoint win;
	win.x = app->win->GetWidth();
	win.y = app->win->GetHeight();
	fPoint pos;
	pos.x = (app->player->pos.x - (win.x / 2)) * parallax;
	pos.y = (app->player->pos.y - (win.y/ 2)) * parallax;

	app->render->DrawTexture(background, (int)pos.x, (int)pos.y, NULL);
	currentMap->Draw();

	iPoint map = currentMap->MapToWorld(currentMap->mapData.width,currentMap->mapData.height);

	
	app->render->DrawRectangle({ -win.x,-win.y,map.x + win.x,win.y }, 0, 0, 0, 255, true, true);
	app->render->DrawRectangle({ 0,map.y,map.x + win.x,win.y }, 0, 0, 0, 255, true, true);
	app->render->DrawRectangle({ -win.x,0,win.x ,win.y + map.y }, 0, 0, 0, 255, true, true);
	app->render->DrawRectangle({ map.x,-win.y,win.x,win.y + map.y }, 0, 0, 0, 255, true, true);
	

	
	ListItem<Platform*>* item = platforms.start;
	while (item != NULL)
	{
		item->data->Update(dt);
		app->render->DrawTexture(platformImg, item->data->pos.x, item->data->pos.y, NULL);

		item = item->next;
	}


	// L03: DONE 7: Set the window title with map/tileset info
	SString title("I see the light");

	app->win->SetTitle(title.GetString());

	return true;
}

// Called each loop iteration
bool SceneGame::PostUpdate()
{
	bool ret = true;

	if (app->input->GetKey(SDL_SCANCODE_F1) == KEY_DOWN)
	{
		app->player->level = 1;
		ChangeMap();
	}
	/*if (app->input->GetKey(SDL_SCANCODE_F2) == KEY_DOWN)
	{
		app->player->level = 2;
		ChangeMap();
	}*/


	if (app->input->GetKey(SDL_SCANCODE_F3) == KEY_DOWN) ChangeMap();

	if (app->input->GetKey(SDL_SCANCODE_F5) == KEY_DOWN) app->LoadGameRequest();
	if (app->input->GetKey(SDL_SCANCODE_F6) == KEY_DOWN) app->SaveGameRequest();


	if(app->input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN)
		ret = false;

	

	return ret;
}

bool SceneGame::CreateCollisions()
{


	return true;
}

// Called before quitting
bool SceneGame::CleanUp()
{
	platforms.clear();

	

	LOG("Freeing scene");

	return true;
}

bool SceneGame::ChangeMap()
{

	// clear scene colliders and platforms

	app->collisions->collidersList.clear();
	platforms.clear();

	if (app->player->level == 1) currentMap = app->map1;
	//if (app->player->level == 2) currentMap = app->map2;

	app->player->CreateColliders();

	if (app->GetLoadGameRequested() == false)currentMap->LoadPositions();
	currentMap->LoadCollisions();
	currentMap->LoadFallingPlatforms();

	borders->listeners[0] = this;
	winCol->listeners[0] = this;

	app->player->alive = true;

	return true;
}