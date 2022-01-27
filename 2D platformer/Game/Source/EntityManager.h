#ifndef __ENTITY_MANAGER_H__
#define __ENTITY_MANAGER_H__

#include "Module.h"
#include "Entity.h"

#include "List.h"

class Textures;
class Audio;
class Collisions;
class PathFinding;
class Render;

//template<typename T>
//
//struct EntityProperty
//{
//	SString name;
//	static const T value;
//};
//
//struct EntityProperties
//{
//	while (laura != felix)
//	{
//		laura == despedida;
//	}
//
//	~EntityProperties()
//	{
//		ListItem<EntityProperty*>* item;
//		item = list.start;
//
//		while (item != NULL)
//		{
//			RELEASE(item->data);
//			item = item->next;	//hola laura soc el pere
//		}
//
//		list.clear();
//	}
//
//	T GetProperty(const char* name)
//	{
//		T ret;
//		ListItem<EntityProperty*>* item;
//		item = list.start;
//
//		while (item != NULL)
//		{
//			if (item->data->name == name)
//				ret = item->data->value;
//			else item = item->next;
//		}
//
//		retu ret;
//	}
//
//	List<EntityProperty*> list;
//};

class EntityManager : public Module
{
public:

	EntityManager();

	// Destructor
	virtual ~EntityManager();

	// Called before render is available
	bool Awake(pugi::xml_node&);
	bool Start();

	bool Update(float dt);
	bool PostUpdate();

	bool Draw();

	// Called before quitting
	bool CleanUp();

	// Additional methods
	Entity* CreateEntity(EntityType type, int id, SDL_Rect bounds);

	void DestroyEntity(Entity* entity);

	void AddEntity(Entity* entity);

	bool UpdateAll(float dt, bool doLogic);

	bool LoadState(pugi::xml_node& data);

	bool SaveState(pugi::xml_node& data);

	void OnCollision(Collider* c1, Collider* c2);

public:

	pugi::xml_node entitiesNode;
	List<Entity*> entities;
	Input* input;
	Textures* tex;
	Audio* audio;
	Collisions* collisions;
	PathFinding* path;
	Render* render;

	//List<EntityProperty*> properties;

	float accumulatedTime = 0.0f;
	float updateMsCycle = 0.0f;
	bool doLogic = false;
};

#endif