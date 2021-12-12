#include "App.h"
#include "Input.h"
#include "Textures.h"
#include "Audio.h"
#include "Render.h"
#include "Window.h"
#include "SceneGame.h"
#include "FadeToBlack.h"
#include "Collisions.h"
#include "Collider.h"
#include "Map.h"
#include "Player.h"
#include "PathFinding.h"

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
	
	backgroundPath = config.attribute("backgroundPath").as_string();
	parallax = config.attribute("parallax").as_float();
	platformPath = config.child("elements").child("platform").attribute("platformPath").as_string();
	walkInfo.spritePath = config.child("elements").child("walkEnemy").attribute("path").as_string();
	flyInfo.spritePath = config.child("elements").child("flyEnemy").attribute("path").as_string();

	return ret;
}

// Called before the first frame
bool SceneGame::Start()
{
	active = false;
	app->map1->Load("level1.tmx");
	app->map2->Load("level2.tmx");
	app->player->active = true;
	background = app->tex->Load(backgroundPath.GetString());
	platformImg = app->tex->Load(platformPath.GetString());
	
	walkInfo.spriteSheet = app->tex->Load(walkInfo.spritePath.GetString());
	flyInfo.spriteSheet= app->tex->Load(flyInfo.spritePath.GetString());

	//pues a crear las animaciones

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


	ListItem<Platform*>* pItem = platforms.start;
	while (pItem != NULL)
	{
		pItem->data->Update(dt);
		app->render->DrawTexture(platformImg, pItem->data->pos.x, pItem->data->pos.y, NULL);

		pItem = pItem->next;
	}

	ListItem<Enemy*>* eItem = enemies.start;
	while (eItem != NULL)
	{
		if (eItem->data->active == true)
		{
			eItem->data->Update(dt);
			eItem = eItem->next;
		}
		else
		{

			ListItem<Enemy*>* del = eItem;
			eItem = del->next;

			//bool cleaned = del->data->CleanUp();



			enemies.del(del);
		}
	}
	
	app->render->DrawRectangle({ -win.x,-win.y,map.x + win.x,win.y }, 0, 0, 0, 255, true, true);
	app->render->DrawRectangle({ 0,map.y,map.x + win.x,win.y }, 0, 0, 0, 255, true, true);
	app->render->DrawRectangle({ -win.x,0,win.x ,win.y + map.y }, 0, 0, 0, 255, true, true);
	app->render->DrawRectangle({ map.x,-win.y,win.x,win.y + map.y }, 0, 0, 0, 255, true, true);
	
	


	// L03: DONE 7: Set the window title with map/tileset info
	SString title("I see the light");

	//app->win->SetTitle(title.GetString());
	
	
	

	return true;
}

// Called each loop iteration
bool SceneGame::PostUpdate()
{
	bool ret = true;


	ListItem<Enemy*>* eItem = enemies.start;
	while (eItem != NULL)
	{
		eItem->data->PostUpdate();
		eItem = eItem->next;
	}



	if (app->input->GetKey(SDL_SCANCODE_F1) == KEY_DOWN)
	{
		app->player->level = 1;
		app->fade->Fade(this, app->sceneGame, app->fade->time / app->dt);
	}

	if (app->input->GetKey(SDL_SCANCODE_F2) == KEY_DOWN)
	{
		app->player->level = 2;
		app->fade->Fade(this, app->sceneGame, app->fade->time / app->dt);
	}


	if (app->input->GetKey(SDL_SCANCODE_F3) == KEY_DOWN) ChangeMap();

	if (app->input->GetKey(SDL_SCANCODE_F6) == KEY_DOWN) app->LoadGameRequest();
	if (app->input->GetKey(SDL_SCANCODE_F5) == KEY_DOWN) app->SaveGameRequest();

	if (app->input->GetKey(SDL_SCANCODE_F11) == KEY_DOWN) app->cap30fps = !app->cap30fps;

	if (app->input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN)
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

	

	LOG("Freeing scene");

	return true;
}

bool SceneGame::ChangeMap()
{
	// hay q revisar esto
	app->collisions->collidersList.clear();
	platforms.clear();
	enemies.clear();

	if (app->player->level == 1) currentMap = app->map1;
	if (app->player->level == 2) currentMap = app->map2;

	app->player->CreateColliders();

	if (app->GetLoadGameRequested() == false)currentMap->LoadPositions();
	currentMap->LoadCollisions();
	currentMap->LoadPlatforms();
	currentMap->LoadEnemies();

	borders->listeners[0] = this;
	winCol->listeners[0] = this;

	app->player->alive = true;

	return true;
}

