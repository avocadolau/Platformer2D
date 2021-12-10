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

	matrix[Collider::Type::BORDER][Collider::Type::BORDER] = false;
	matrix[Collider::Type::BORDER][Collider::Type::GROUND] = false;
	matrix[Collider::Type::BORDER][Collider::Type::PLAYER] = true;
	matrix[Collider::Type::BORDER][Collider::Type::WIN] = false;
	matrix[Collider::Type::BORDER][Collider::Type::NONE] = false;

	matrix[Collider::Type::GROUND][Collider::Type::BORDER] = false;
	matrix[Collider::Type::GROUND][Collider::Type::GROUND] = false;
	matrix[Collider::Type::GROUND][Collider::Type::PLAYER] = true;
	matrix[Collider::Type::GROUND][Collider::Type::WIN] = false;
	matrix[Collider::Type::GROUND][Collider::Type::NONE] = false;

	matrix[Collider::Type::PLAYER][Collider::Type::BORDER] = true;
	matrix[Collider::Type::PLAYER][Collider::Type::GROUND] = true;
	matrix[Collider::Type::PLAYER][Collider::Type::PLAYER] = false;
	matrix[Collider::Type::PLAYER][Collider::Type::WIN] = true;
	matrix[Collider::Type::PLAYER][Collider::Type::NONE] = true;

	matrix[Collider::Type::WIN][Collider::Type::BORDER] = false;
	matrix[Collider::Type::WIN][Collider::Type::GROUND] = false;
	matrix[Collider::Type::WIN][Collider::Type::PLAYER] = true;
	matrix[Collider::Type::WIN][Collider::Type::WIN] = false;
	matrix[Collider::Type::WIN][Collider::Type::NONE] = false;

	matrix[Collider::Type::NONE][Collider::Type::BORDER] = false;
	matrix[Collider::Type::NONE][Collider::Type::GROUND] = false;
	matrix[Collider::Type::NONE][Collider::Type::PLAYER] = true;
	matrix[Collider::Type::NONE][Collider::Type::WIN] = false;
	matrix[Collider::Type::NONE][Collider::Type::NONE] = false;

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

Collider* Collisions::AddCollider(SDL_Rect rect, Collider::Type type, Module* listener)
{
	Collider* newCol = new Collider(rect, type, listener);
	collidersList.add(newCol);

	return newCol;
}

void Collisions::RemoveCollider(Collider* collider)
{
	/*for (uint i = 0; i < MAX_COLLIDERS; ++i)
	{
		if (colliders[i] == collider)
		{
			delete colliders[i];
			colliders[i] = nullptr;
		}
	}*/

	ListItem<Collider*>* item;
	item = collidersList.start;

	while (item != NULL)
	{
		if (item->data == collider) collidersList.del(item);
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
		case Collider::Type::LISTENER: // blue
			app->render->DrawRectangle(col->data->rect, 0, 0, 255, alpha, true, true);
			break;
		case Collider::Type::GROUND: // yellow
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