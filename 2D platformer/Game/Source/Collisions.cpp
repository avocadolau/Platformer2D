#include "Collisions.h"

#include "App.h"
#include "Input.h"
#include "Render.h"
#include "Window.h"
#include "Collider.h"

Collisions::Collisions() :Module()
{
	for (uint i = 0; i < MAX_COLLIDERS; ++i)
		colliders[i] = nullptr;

	matrix[Collider::Type::LISTENER][Collider::Type::LISTENER] = false;
	matrix[Collider::Type::LISTENER][Collider::Type::GROUND] = true;
	matrix[Collider::Type::LISTENER][Collider::Type::PLAYER] = false;
	matrix[Collider::Type::LISTENER][Collider::Type::WIN] = false;
	matrix[Collider::Type::LISTENER][Collider::Type::DIE] = false;

	matrix[Collider::Type::GROUND][Collider::Type::LISTENER] = true;
	matrix[Collider::Type::GROUND][Collider::Type::GROUND] = true;
	matrix[Collider::Type::GROUND][Collider::Type::PLAYER] = true;
	matrix[Collider::Type::GROUND][Collider::Type::WIN] = false;
	matrix[Collider::Type::GROUND][Collider::Type::DIE] = false;

	matrix[Collider::Type::PLAYER][Collider::Type::LISTENER] = false;
	matrix[Collider::Type::PLAYER][Collider::Type::GROUND] = true;
	matrix[Collider::Type::PLAYER][Collider::Type::PLAYER] = false;
	matrix[Collider::Type::PLAYER][Collider::Type::WIN] = true;
	matrix[Collider::Type::PLAYER][Collider::Type::DIE] = true;

	matrix[Collider::Type::WIN][Collider::Type::LISTENER] = false;
	matrix[Collider::Type::WIN][Collider::Type::GROUND] = false;
	matrix[Collider::Type::WIN][Collider::Type::PLAYER] = true;
	matrix[Collider::Type::WIN][Collider::Type::WIN] = false;
	matrix[Collider::Type::WIN][Collider::Type::DIE] = false;

	matrix[Collider::Type::DIE][Collider::Type::LISTENER] = false;
	matrix[Collider::Type::DIE][Collider::Type::GROUND] = false;
	matrix[Collider::Type::DIE][Collider::Type::PLAYER] = true;
	matrix[Collider::Type::DIE][Collider::Type::WIN] = false;
	matrix[Collider::Type::DIE][Collider::Type::DIE] = false;

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
	Collider* c1;
	Collider* c2;

	for (uint i = 0; i < MAX_COLLIDERS; ++i)
	{
		// skip empty colliders
		if (colliders[i] == nullptr)
			continue;

		c1 = colliders[i];

		// avoid checking collisions already checked
		for (uint k = i + 1; k < MAX_COLLIDERS; ++k)
		{
			// skip empty colliders
			if (colliders[k] == nullptr)
				continue;

			c2 = colliders[k];

			if (matrix[c1->type][c2->type] && c1->Intersects(c2->rect))
			{
				for (uint i = 0; i < MAX_LISTENERS; ++i)
					if (c1->listeners[i] != nullptr) c1->listeners[i]->OnCollision(c1, c2);

				for (uint i = 0; i < MAX_LISTENERS; ++i)
					if (c2->listeners[i] != nullptr) c2->listeners[i]->OnCollision(c2, c1);
			}
		}
	}

	return true;
}

bool Collisions::Update(float dt)
{
	if (app->input->GetKey(SDL_SCANCODE_F9) == KEY_DOWN)
		debug = !debug;

	return true;
}

bool Collisions::PostUpdate()
{
	if (debug)
		DebugDraw();

	return true;
}

bool Collisions::CleanUp()
{
	for (uint i = 0; i < MAX_COLLIDERS; ++i)
	{
		if (colliders[i] != nullptr)
		{
			delete colliders[i];
			colliders[i] = nullptr;
		}
	}

	return true;
}

Collider* Collisions::AddCollider(SDL_Rect rect, Collider::Type type, Module* listener)
{
	Collider* ret = nullptr;

	for (uint i = 0; i < MAX_COLLIDERS; ++i)
	{
		if (colliders[i] == nullptr)
		{
			ret = colliders[i] = new Collider(rect, type, listener);
			break;
		}
	}

	return ret;
}

void Collisions::RemoveCollider(Collider* collider)
{
	for (uint i = 0; i < MAX_COLLIDERS; ++i)
	{
		if (colliders[i] == collider)
		{
			delete colliders[i];
			colliders[i] = nullptr;
		}
	}
}

void Collisions::DebugDraw()
{
	//
	Uint8 alpha = 80;
	for (uint i = 0; i < MAX_COLLIDERS; ++i)
	{
		if (colliders[i] == nullptr)
			continue;

		switch (colliders[i]->type)
		{
		case Collider::Type::NONE: // white
			app->render->DrawRectangle(colliders[i]->rect, 255, 255, 255, alpha, true, true);
			break;
		case Collider::Type::LISTENER: // blue
			app->render->DrawRectangle(colliders[i]->rect, 0, 0, 255, alpha, true, true);
			break;
		case Collider::Type::GROUND: // yellow
			app->render->DrawRectangle(colliders[i]->rect, 255, 255, 0, alpha, true, true);
			break;
		case Collider::Type::PLAYER: // purple
			app->render->DrawRectangle(colliders[i]->rect, 122, 0, 255, alpha, true, true);
		break;
		case Collider::Type::WIN: // green
			app->render->DrawRectangle(colliders[i]->rect, 0, 255, 0, alpha, true, true);
			break;
		case Collider::Type::DIE: // orange
			app->render->DrawRectangle(colliders[i]->rect, 255, 122, 0, alpha, true, true);
			break;
		}
	}
}