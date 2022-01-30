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
#include "Checkpoint.h"
#include "EntityManager.h"
#include "Entity.h"
#include "Coin.h"
#include "GuiManager.h"
#include "GuiControl.h"
#include "FadeToBlack.h"
#include "SceneStart.h"

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
	level = config.attribute("level").as_int();

	return ret;
}

// Called before the first frame
bool SceneGame::Start()
{
	active = false;
	app->map1->Load("level1.tmx");
	app->map2->Load("level2.tmx");

	if (app->player == NULL) {
		SDL_Rect rec = { 0,0,0,0 };
		app->entityManager->CreateEntity(EntityType::PLAYER, 0, rec);
	}

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
	//app->checkpoint->active = true;
	return true;
}

// Called each loop iteration
bool SceneGame::Update(float dt)
{
	if (pause == false) timer += dt / 1000;
	if ((int)app->player->scoreHud < app->player->score)
	{
		app->player->scoreHud += dt / 200;
		app->guiManager->fontColor = 1;
		
	}
	else app->guiManager->fontColor = 0;


	if (currentMap == app->map1)
	{
		if (maxScoreLvl1 < app->player->scoreHud) maxScoreLvl1 = app->player->scoreHud;
		highScore = maxScoreLvl1;
	}
		
	if (currentMap == app->map2)
	{
		if (maxScoreLvl2 < app->player->scoreHud) maxScoreLvl2 = app->player->scoreHud;
		highScore = maxScoreLvl2;
	}

	if (app->player->alive == false) {
		app->player->scoreHud = 0;
	}

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
	
	

	// L03: DONE 7: Set the window title with map/tileset info
	SString title("I see the light");

	//app->win->SetTitle(title.GetString());
	
	if (mainMenu == true)
	{
		mainMenu = false;
		app->fade->Fade(this, app->sceneStart, app->fade->time / dt);

	}
	

	return true;
}

// Called each loop iteration
bool SceneGame::PostUpdate()
{
	bool ret = true;

	if (app->input->GetKey(SDL_SCANCODE_F1) == KEY_DOWN)
	{
		level = 1;
		app->fade->Fade(this, app->sceneGame, app->fade->time / app->dt);
	}

	if (app->input->GetKey(SDL_SCANCODE_F2) == KEY_DOWN)
	{
		level = 2;
		app->fade->Fade(this, app->sceneGame, app->fade->time / app->dt);
	}


	if (app->input->GetKey(SDL_SCANCODE_F3) == KEY_DOWN) ChangeMap();

	if (app->input->GetKey(SDL_SCANCODE_F6) == KEY_DOWN) app->LoadGameRequest();
	if (app->input->GetKey(SDL_SCANCODE_F5) == KEY_DOWN) app->SaveGameRequest();

	if (app->input->GetKey(SDL_SCANCODE_F10) == KEY_DOWN) app->player->godMode = !app->player->godMode;

	if (app->input->GetKey(SDL_SCANCODE_F11) == KEY_DOWN) app->cap30fps = !app->cap30fps;

	if (app->input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN)
	{
		if (pause == false)
			pause = true;


		//else ret = false;
	}


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
	
	app->player->Disable();
	RemoveGroundColliders();
	app->collisions->RemoveCollider(winCol);
	app->collisions->RemoveCollider(borders);
	
	app->entityManager->RemoveEntities();

	if (level == 1) currentMap = app->map1;
	if (level == 2) currentMap = app->map2;

	//app->player->CreateColliders();

	if (app->GetLoadGameRequested() == false)
		currentMap->LoadPositions();

	currentMap->LoadCollisions();
	currentMap->LoadPlatforms();
	currentMap->LoadEnemies();
	currentMap->LoadCoins();

	borders->listeners[0] = this;
	winCol->listeners[0] = this;

	app->player->Enable();

	return true;
}

bool SceneGame::RemoveGroundColliders()
{
	ListItem<Collider*>* p = app->collisions->collidersList.start;

	while (p != NULL)
	{
		if (p->data->type == Collider::Type::GROUND)
		{
			ListItem<Collider*>* del = p;
			p = p->next;
			app->collisions->RemoveCollider(del->data);
		}
		else p = p->next;
	}
	return true;
}

bool SceneGame::LoadState(pugi::xml_node& node)
{
	level = node.attribute("level").as_int();
	/*deadEnemies.clear();
	for (pugi::xml_node eNode = node.child("enemies").child("id"); eNode ; eNode = eNode.next_sibling("id"))
	{
		int id= eNode.attribute("value").as_int();
		deadEnemies.add(&id);
	}
	destroyDeadEnemies = true;*/
	return true;
}

bool SceneGame::SaveState(pugi::xml_node& node) const
{
	/*pugi::xml_node enemies = node.append_child("enemies");
	
	node.append_attribute("level").set_value(level);
	
	ListItem<int*>* item = deadEnemies.start;
	while (item != NULL)
	{
		pugi::xml_node id = enemies.append_child("id");
		id.append_attribute("value").set_value(item->data);
		item = item->next;
	}*/
	return true;
}

void SceneGame::OnCollision(Collider* c1, Collider* c2)
{
	

}

bool SceneGame::OnGuiMouseClickEvent(GuiControl* control)
{

	switch (control->id)
	{
	case 6:		pause = false;				break;
	case 8:		mainMenu = true;			break;
	case 9:		app->exit = true;			break;

	default:
		break;
	}


	

	return true;
}