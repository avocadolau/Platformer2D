#include "EntityManager.h"
#include "App.h"
#include "Module.h"

#include "Entity.h"
#include "Player.h"
#include "Coin.h"

#include "Defs.h"
#include "Log.h"

EntityManager::EntityManager() : Module()
{
	name.Create("entityManager");
}

// Destructor
EntityManager::~EntityManager()
{
	CleanUp();
}

// Called before render is available
bool EntityManager::Awake(pugi::xml_node& config)
{
	LOG("Loading Entity Manager");

	pugi::xml_node node;

	// player
	node = config.child("player");

	playerProperties.lives = node.attribute("lives").as_int();
	playerProperties.maxVel.x = node.child("physics").attribute("maxVelX").as_float();
	playerProperties.maxVel.y = node.child("physics").attribute("maxVelY").as_float();
	playerProperties.gravity = node.child("physics").attribute("gravity").as_float();
	playerProperties.dim.x = node.child("dimensions").attribute("x").as_int();
	playerProperties.dim.y = node.child("dimensions").attribute("y").as_int();

	playerProperties.animSpeed = node.child("animations").attribute("speed").as_float();
	playerProperties.spritePath = ("%s", node.child("animations").attribute("path").as_string());

	playerProperties.idleW = node.child("animations").child("idle").attribute("width").as_int();
	playerProperties.idleH = node.child("animations").child("idle").attribute("height").as_int();
	playerProperties.idleN = node.child("animations").child("idle").attribute("sprites").as_int();

	playerProperties.runW = node.child("animations").child("run").attribute("width").as_int();
	playerProperties.runH = node.child("animations").child("run").attribute("height").as_int();
	playerProperties.runN = node.child("animations").child("run").attribute("sprites").as_int();

	playerProperties.jumpW = node.child("animations").child("jump").attribute("width").as_int();
	playerProperties.jumpH = node.child("animations").child("jump").attribute("height").as_int();
	playerProperties.jumpN = node.child("animations").child("jump").attribute("sprites").as_int();

	playerProperties.deathW = node.child("animations").child("death").attribute("width").as_int();
	playerProperties.deathH = node.child("animations").child("death").attribute("height").as_int();
	playerProperties.deathN = node.child("animations").child("death").attribute("sprites").as_int();

	// enemies


	// platforms
	elements.platformPath = config.child("platform").attribute("path").as_string();

	// coins
	node = config.child("coin");
	coinProperties.dim= node.attribute("r").as_int();

	// shot


	bool ret = true;

	//L13: TODO 6: Initialize Entities from XML

	return ret;
}

bool EntityManager::Start()
{
	render = app->render;

	app->entityManager->coinProperties.sprite = app->tex->Load(app->entityManager->coinProperties.path.GetString());
	app->entityManager->elements.platformImg = app->tex->Load(app->entityManager->elements.platformPath.GetString());

	return true;
}

// Called before quitting
bool EntityManager::CleanUp()
{
	bool ret = true;
	ListItem<Entity*>* item;
	item = entities.end;

	while (item != NULL && ret == true)
	{
		ret = item->data->CleanUp();
		item = item->prev;
	}

	entities.clear();

	return ret;
}

Entity* EntityManager::CreateEntity(EntityType type, int id, SDL_Rect bounds)
{
	Entity* entity = nullptr;

	//L13: TODO 1: Create an Entity and add it to the list of Entities
	switch (type)
	{
	case EntityType::PLAYER:		entity = new Player(); break;
	case EntityType::COIN:			entity = new Coin(id, bounds); break;
	case EntityType::PLATFORM:		entity = new Platform({ bounds.x, bounds.y }); break;
	case EntityType::ATACK:			entity = new PlayerAtack(bounds); break;
	default:						break;
	}

	AddEntity(entity);

	return entity;
}

void EntityManager::DestroyEntity(Entity* entity)
{
	ListItem<Entity*>* item;

	for (item = entities.start; item != NULL; item = item->next)
	{
		if (item->data == entity) entities.del(item);
	}
}

void EntityManager::AddEntity(Entity* entity)
{
	if (entity != nullptr) entities.add(entity);
	entity->Start();
}

bool EntityManager::Update(float dt)
{
	accumulatedTime += dt;
	if (accumulatedTime >= updateMsCycle) doLogic = true;

	UpdateAll(dt, doLogic);

	if (doLogic == true)
	{
		accumulatedTime = 0.0f;
		doLogic = false;
	}

	return true;
}



bool EntityManager::UpdateAll(float dt, bool doLogic)
{
	bool ret = true;
	ListItem<Entity*>* item;
	Entity* pEntity = NULL;

	if (doLogic)
	{
		for (item = entities.start; item != NULL && ret == true; item = item->next)
		{
			pEntity = item->data;

			if (pEntity->active == true)
				item->data->Update(dt);
		}
	}

	item = entities.start;
	while (item != NULL)
	{
		pEntity = item->data;
		
		if (pEntity->toDestroy == true)
		{
			item = item->next;
			DestroyEntity(pEntity);
		}
		else item = item->next;
	}

	return ret;
}

bool EntityManager::PostUpdate()
{
	bool ret = true;
	ListItem<Entity*>* item;
	Entity* pEntity = NULL;

	for (item = entities.start; item != NULL && ret == true; item = item->next)
	{
		pEntity = item->data;
		if (pEntity->active == false) continue;
		if (pEntity->Draw(render) == false) ret = false;
	}

	return ret;
}


bool EntityManager::LoadState(pugi::xml_node& data)
{
	bool ret = true;

	ListItem<Entity*>* item;
	item = entities.start;

	while (item != NULL && ret == true)
	{
		ret = item->data->Load(data.child(item->data->name.GetString()));
		item = item->next;
	}

	return ret;
}

bool EntityManager::SaveState(pugi::xml_node& data)
{
	bool ret = true;

	ListItem<Entity*>* item;
	item = entities.start;

	while (item != NULL && ret == true)
	{
		pugi::xml_node node = data.append_child("position");
		//data.append_child(item->data->name.GetString());
		ret = item->data->Save(node);
		item = item->next;
	}

	return ret;
}

bool EntityManager::Draw() {

	bool ret = true;
	ListItem<Entity*>* item;
	Entity* pEntity = NULL;

	for (item = entities.start; item != NULL && ret == true; item = item->next)
	{
		pEntity = item->data;

		if (pEntity->active == false) continue;
		ret = item->data->Draw(app->render);
	}

	return ret;

}

void EntityManager::OnCollision(Collider* c1, Collider* c2)
{

	if (c1->entity != nullptr)
		if (c1->entity->active==true)
			c1->entity->OnCollision(c1, c2);

	
}