#include "Collisions.h"

#include "App.h"
#include "Input.h"
#include "Render.h"
#include "Window.h"
#include "Collider.h"
#include "SceneGame.h"

Collisions::Collisions() :Module()
{
	name.Create("collisions");

	matrix[Collider::Type::DETECTOR][Collider::Type::NONE] = false;
	matrix[Collider::Type::DETECTOR][Collider::Type::DETECTOR] = false;
	matrix[Collider::Type::DETECTOR][Collider::Type::GROUND] = false;
	matrix[Collider::Type::DETECTOR][Collider::Type::PLAYER] = true;
	matrix[Collider::Type::DETECTOR][Collider::Type::PATACK] = false;
	matrix[Collider::Type::DETECTOR][Collider::Type::ENEMY] = true;
	matrix[Collider::Type::DETECTOR][Collider::Type::WIN] = false;
	matrix[Collider::Type::DETECTOR][Collider::Type::BORDER] = false;
	matrix[Collider::Type::DETECTOR][Collider::Type::PICKUP] = false;

	matrix[Collider::Type::GROUND][Collider::Type::NONE] = false;
	matrix[Collider::Type::GROUND][Collider::Type::DETECTOR] = false;
	matrix[Collider::Type::GROUND][Collider::Type::GROUND] = false;
	matrix[Collider::Type::GROUND][Collider::Type::PLAYER] = true;
	matrix[Collider::Type::GROUND][Collider::Type::PATACK] = true;
	matrix[Collider::Type::GROUND][Collider::Type::ENEMY] = false;
	matrix[Collider::Type::GROUND][Collider::Type::WIN] = false;
	matrix[Collider::Type::GROUND][Collider::Type::BORDER] = false;
	matrix[Collider::Type::GROUND][Collider::Type::PICKUP] = false;

	matrix[Collider::Type::PLAYER][Collider::Type::NONE] = true;
	matrix[Collider::Type::PLAYER][Collider::Type::DETECTOR] = true;
	matrix[Collider::Type::PLAYER][Collider::Type::GROUND] = true;
	matrix[Collider::Type::PLAYER][Collider::Type::PLAYER] = false;
	matrix[Collider::Type::PLAYER][Collider::Type::PATACK] = false;
	matrix[Collider::Type::PLAYER][Collider::Type::ENEMY] = true;
	matrix[Collider::Type::PLAYER][Collider::Type::WIN] = true;
	matrix[Collider::Type::PLAYER][Collider::Type::BORDER] = true;
	matrix[Collider::Type::PLAYER][Collider::Type::PICKUP] = true;

	matrix[Collider::Type::PATACK][Collider::Type::NONE] = false;
	matrix[Collider::Type::PATACK][Collider::Type::DETECTOR] = false;
	matrix[Collider::Type::PATACK][Collider::Type::GROUND] = true;
	matrix[Collider::Type::PATACK][Collider::Type::PLAYER] = false;
	matrix[Collider::Type::PATACK][Collider::Type::PATACK] = false;
	matrix[Collider::Type::PATACK][Collider::Type::ENEMY] = true;
	matrix[Collider::Type::PATACK][Collider::Type::WIN] = false;
	matrix[Collider::Type::PATACK][Collider::Type::BORDER] = false;
	matrix[Collider::Type::PATACK][Collider::Type::BORDER] = false;


	matrix[Collider::Type::ENEMY][Collider::Type::NONE] = false;
	matrix[Collider::Type::ENEMY][Collider::Type::DETECTOR] = true;
	matrix[Collider::Type::ENEMY][Collider::Type::GROUND] = false;
	matrix[Collider::Type::ENEMY][Collider::Type::PLAYER] = true;
	matrix[Collider::Type::ENEMY][Collider::Type::PATACK] = true;
	matrix[Collider::Type::ENEMY][Collider::Type::ENEMY] = false;
	matrix[Collider::Type::ENEMY][Collider::Type::WIN] = false;
	matrix[Collider::Type::ENEMY][Collider::Type::BORDER] = false;
	matrix[Collider::Type::ENEMY][Collider::Type::PICKUP] = false;


	matrix[Collider::Type::WIN][Collider::Type::NONE] = false;
	matrix[Collider::Type::WIN][Collider::Type::DETECTOR] = false;
	matrix[Collider::Type::WIN][Collider::Type::GROUND] = false;
	matrix[Collider::Type::WIN][Collider::Type::PLAYER] = true;
	matrix[Collider::Type::WIN][Collider::Type::PATACK] = false;
	matrix[Collider::Type::WIN][Collider::Type::ENEMY] = false;
	matrix[Collider::Type::WIN][Collider::Type::WIN] = false;
	matrix[Collider::Type::WIN][Collider::Type::BORDER] = false;
	matrix[Collider::Type::WIN][Collider::Type::PICKUP] = false;

	matrix[Collider::Type::BORDER][Collider::Type::NONE] = false;
	matrix[Collider::Type::BORDER][Collider::Type::DETECTOR] = false;
	matrix[Collider::Type::BORDER][Collider::Type::GROUND] = false;
	matrix[Collider::Type::BORDER][Collider::Type::PLAYER] = true;
	matrix[Collider::Type::BORDER][Collider::Type::PATACK] = false;
	matrix[Collider::Type::BORDER][Collider::Type::ENEMY] = false;
	matrix[Collider::Type::BORDER][Collider::Type::WIN] = false;
	matrix[Collider::Type::BORDER][Collider::Type::BORDER] = false;
	matrix[Collider::Type::BORDER][Collider::Type::PICKUP] = false;

	matrix[Collider::Type::PICKUP][Collider::Type::NONE] = false;
	matrix[Collider::Type::PICKUP][Collider::Type::DETECTOR] = false;
	matrix[Collider::Type::PICKUP][Collider::Type::GROUND] = false;
	matrix[Collider::Type::PICKUP][Collider::Type::PLAYER] = true;
	matrix[Collider::Type::PICKUP][Collider::Type::PATACK] = false;
	matrix[Collider::Type::PICKUP][Collider::Type::ENEMY] = false;
	matrix[Collider::Type::PICKUP][Collider::Type::WIN] = false;
	matrix[Collider::Type::PICKUP][Collider::Type::BORDER] = false;
	matrix[Collider::Type::PICKUP][Collider::Type::PICKUP] = false;

}

Collisions::~Collisions()
{

}

bool Collisions::Awake()
{

	return true;
}

bool Collisions::Start()
{

	return true;
}

bool Collisions::PreUpdate()
{

	return true;
}

bool Collisions::Update(float dt)
{
	if (app->input->GetKey(SDL_SCANCODE_F9) == KEY_DOWN)
		app->debug = !app->debug;

	if (collidersList.start == NULL) return true;

	ListItem<Collider*>* c1;
	c1 = collidersList.start;

	while (c1 != NULL)
	{
		ListItem<Collider*>* c2;
		c2 = c1->next;

		while (c2 != NULL)
		{
			if (matrix[c1->data->type][c2->data->type])
				if (c1->data->Intersects(c2->data->rect))
				{

					for (uint i = 0; i < MAX_LISTENERS; ++i)
						if (c1->data->listeners[i] != nullptr) c1->data->listeners[i]->OnCollision(c1->data, c2->data);

					for (uint i = 0; i < MAX_LISTENERS; ++i)
						if (c2->data->listeners[i] != nullptr) c2->data->listeners[i]->OnCollision(c2->data, c1->data);
				}
			c2 = c2->next;
		}
		c1 = c1->next;
	}

	return true;
}

bool Collisions::PostUpdate()
{
	if (app->sceneGame->active == false) app->debug = false;
	if (app->debug)
		DebugDraw();

	return true;
}

bool Collisions::CleanUp()
{

	collidersList.clear();

	return true;
}

Collider* Collisions::AddCollider(SDL_Rect rect, Collider::Type type, Module* listener, Entity*entity)
{
	Collider* newCol = new Collider(rect, type, listener, entity);
	collidersList.add(newCol);

	return newCol;
}

void Collisions::RemoveCollider(Collider* collider)
{

	ListItem<Collider*>* item;
	item = collidersList.start;

	while (item != NULL)
	{
		if (item->data == collider)
		{
			collidersList.del(item);
			break;
		}
		item = item->next;
	}

	
}

void Collisions::DebugDraw()
{
	//
	Uint8 alpha = 80;

	ListItem<Collider*>* col;
	col = collidersList.start;

	while (col!=NULL)
	{
		switch (col->data->type)
		{
		case Collider::Type::NONE: // white
			app->render->DrawRectangle(col->data->rect, 255, 255, 255, alpha, true, true);
			break;
		case Collider::Type::DETECTOR: // CLEAR BLUE
			app->render->DrawRectangle(col->data->rect, 119, 224, 231, alpha, true, true);
			break;
		case Collider::Type::GROUND: // RED
			app->render->DrawRectangle(col->data->rect, 255, 0, 0, alpha, true, true);
			break;
		case Collider::Type::ENEMY: // RED
			app->render->DrawRectangle(col->data->rect, 255, 0, 0, alpha, true, true);
			break;
		case Collider::Type::PLAYER: // purple
			app->render->DrawRectangle(col->data->rect, 122, 0, 255, alpha, true, true);
			break;
		case Collider::Type::WIN: // green
			app->render->DrawRectangle(col->data->rect, 0, 255, 0, alpha, true, true);
			break;
		case Collider::Type::BORDER: // orange
			app->render->DrawRectangle(col->data->rect, 255, 122, 0, alpha, false, true);
			break;
		}

		col = col->next;
	}
}