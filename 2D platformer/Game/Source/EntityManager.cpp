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


	// enemies


	// platforms

	// coins

	// shot


	bool ret = true;

	//L13: TODO 6: Initialize Entities from XML

	return ret;
}

bool EntityManager::Start()
{
	render = app->render;

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
	case EntityType::FLY_ENEMY:		break;
	case EntityType::WALK_ENEMY:	break;
	case EntityType::COIN:			entity = new Coin(id, bounds); break;
	case EntityType::PLATFORM:		break;

	default:
		break;
	}

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

			if (pEntity->active == false)
			{
				if (pEntity != app->player)
				{
					item = item->prev;
					DestroyEntity(pEntity);
				}
			}
				
			else ret = item->data->Update(dt);
		}
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
		data.append_child(item->data->name.GetString());
		ret = item->data->Save(data.child(item->data->name.GetString()));
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
	ListItem<Entity*>* item;
	Entity* pEntity = NULL;

	app->debug = true;

	if (c1->entity != nullptr)
		if (c1->entity->active==true)
			c1->entity->OnCollision(c1, c2);

	
}