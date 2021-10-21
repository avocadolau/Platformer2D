#include "Collisions.h"

#include "App.h"
#include "Collider.h"

#include "Render.h"
#include "Input.h"
#include "SDL/include/SDL_Scancode.h"

Collisions::Collisions() : Module()
{
	for (uint i = 0; i < MAX_COLLIDERS; ++i)
		colliders[i] = nullptr;

	matrix[Collider::Type::PLAYER][Collider::Type::PLAYER] = false;
	matrix[Collider::Type::PLAYER][Collider::Type::PLAYER_FEET] = false;
	matrix[Collider::Type::PLAYER][Collider::Type::GROUND] = true;

	matrix[Collider::Type::PLAYER_FEET][Collider::Type::PLAYER] = false;
	matrix[Collider::Type::PLAYER_FEET][Collider::Type::PLAYER_FEET] = false;
	matrix[Collider::Type::PLAYER_FEET][Collider::Type::GROUND] = true;

	matrix[Collider::Type::GROUND][Collider::Type::PLAYER] = true;
	matrix[Collider::Type::GROUND][Collider::Type::PLAYER_FEET] = true;
	matrix[Collider::Type::GROUND][Collider::Type::GROUND] = false;

}

// Destructor
Collisions::~Collisions()
{

}

bool Collisions::PreUpdate()
{
	// Remove all colliders scheduled for deletion
	for (uint i = 0; i < MAX_COLLIDERS; ++i)
	{
		if (colliders[i] != nullptr)
		{
			if (colliders[i]->pendingToDelete == true) {
				delete colliders[i];
				colliders[i] = nullptr;
			}
		}
	}

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

bool Collisions::Update()
{
	if (app->input->GetKey(SDL_SCANCODE_F9) == KEY_REPEAT)
		debug = !debug;

	if (app->input->GetKey(SDL_SCANCODE_F10) == KEY_DOWN)
		active = !active;

	return true;
}

bool Collisions::PostUpdate()
{
	if (debug)
		DebugDraw();

	return true;
}

void Collisions::DebugDraw()
{
	Uint8 alpha = 50;
	for (uint i = 0; i < MAX_COLLIDERS; ++i)
	{
		if (colliders[i] == nullptr)
			continue;
		bool d;
		switch (colliders[i]->type)
		{
		case Collider::Type::NONE: // white
			d = app->render->DrawRectangle(colliders[i]->rect,225,255,255,alpha,true,true);
			break;
		case Collider::Type::PLAYER:
			d = app->render->DrawRectangle(colliders[i]->rect, 0, 0, 255, alpha, true, true);
			break;
		case Collider::Type::PLAYER_FEET:
			d = app->render->DrawRectangle(colliders[i]->rect, 0, 255, 0, alpha, true, true);
			break;
		case Collider::Type::GROUND:
			d = app->render->DrawRectangle(colliders[i]->rect, 0, 0, 255, alpha, true, true);
			break;
		}
	}
}

// Called before quitting
bool Collisions::CleanUp()
{
	//LOG("Freeing all colliders");

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