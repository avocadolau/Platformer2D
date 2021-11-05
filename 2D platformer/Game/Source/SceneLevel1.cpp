#include "App.h"
#include "Input.h"
#include "Textures.h"
#include "Audio.h"
#include "Render.h"
#include "Window.h"
#include "SceneLevel1.h"
#include "Collisions.h"
#include "Collider.h"
#include "Map.h"
#include "Player.h"

#include "Defs.h"
#include "Log.h"

SceneLevel1::SceneLevel1() : Module()
{
	name.Create("level1");
}

// Destructor
SceneLevel1::~SceneLevel1()
{}

// Called before render is available
bool SceneLevel1::Awake(pugi::xml_node& config)
{
	LOG("Loading Scene");
	bool ret = true;
	
	platformPath = config.attribute("platformPath").as_string();
	backgroundPath = config.child("background").attribute("path").as_string();

	return ret;
}

// Called before the first frame
bool SceneLevel1::Start()
{
	// L03: DONE: Load map
	active = false;
	//app->map->Load("hello.tmx");
	app->map->Load("level1.tmx");
	app->player->active = true;
	platformImg = app->tex->Load(platformPath.GetString());
	background = app->tex->Load(backgroundPath.GetString());

	ListItem<Platform*>* item = platforms.start;
	while (item != NULL)
	{
		item->data->col->listeners[0] = this;

		item = item->next;
	}


	//ground = app->collisions->AddCollider({ 0,400,500,50 }, Collider::Type::GROUND, this);

	// Load music
	//app->audio->PlayMusic("Assets/audio/music/music_spy.ogg");

	return true;
}

// Called each loop iteration
bool SceneLevel1::PreUpdate()
{
	app->player->active = true;
	return true;
}

// Called each loop iteration
bool SceneLevel1::Update(float dt)
{

    // L02: DONE 3: Request Load / Save when pressing L/S
	if(app->input->GetKey(SDL_SCANCODE_L) == KEY_DOWN)
		app->LoadGameRequest();

	if(app->input->GetKey(SDL_SCANCODE_S) == KEY_DOWN)
		app->SaveGameRequest();

	

	//app->render->DrawTexture(img, 380, 100); // Placeholder not needed any more

	// Draw map

	app->map->Draw();
	
	ListItem<Platform*>* item = platforms.start;
	while (item != NULL)
	{
		item->data->Update(dt);
		app->render->DrawTexture(platformImg, item->data->pos.x, item->data->pos.y, NULL);

		item = item->next;
	}


	// L03: DONE 7: Set the window title with map/tileset info
	SString title("Map:%dx%d Tiles:%dx%d Tilesets:%d",
				   app->map->mapData.width, app->map->mapData.height,
				   app->map->mapData.tileWidth, app->map->mapData.tileHeight,
				   app->map->mapData.tilesets.count());

	app->win->SetTitle(title.GetString());

	return true;
}

// Called each loop iteration
bool SceneLevel1::PostUpdate()
{
	bool ret = true;

	if(app->input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN)
		ret = false;

	return ret;
}

bool SceneLevel1::CreateCollisions()
{


	return true;
}

// Called before quitting
bool SceneLevel1::CleanUp()
{
	platforms.clear();
	app->collisions->RemoveCollider(winCol);
	winCol = nullptr;
	app->collisions->RemoveCollider(borders);
	borders = nullptr;

	

	LOG("Freeing scene");

	return true;
}