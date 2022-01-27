#ifndef __ENTITY_H__
#define __ENTITY_H__

#include "Point.h"
#include "SString.h"
#include "Input.h"
#include "Render.h"

struct Collider;

enum class EntityType
{
	PLAYER,
	FLY_ENEMY,
	WALK_ENEMY,
	COIN,
	PLATFORM,
	LIFE,
	UNKNOWN
};

class Entity
{
public:

	Entity(EntityType type) : type(type) {}

	virtual bool Update(float dt)
	{
		return true;
	}

	virtual bool Draw(Render* render)
	{
		return true;
	}

	virtual bool Start()
	{
		return true;
	}

	virtual void Entity::Enable(){}

	virtual void Entity::Disable() {}

	virtual bool Entity::Load(pugi::xml_node&) { return true; }
	virtual bool Entity::Save(pugi::xml_node&) { return true; }

	virtual bool Entity::CleanUp() { return true; }

	virtual void Entity::OnCollision(Collider* c1, Collider* c2) {}

public:
	SString name;
	bool active = false;

	EntityType type;
	//SString name;         // Entity name identifier?
	uint32 id;            // Entity identifier?

	// Possible properties, it depends on how generic we
	// want our Entity class, maybe it's not renderable...
	fPoint position;
	Collider* col;
	//SDL_Texture* texture;
};

#endif // __ENTITY_H__