#include "Coin.h"
#include "App.h"
#include "Textures.h"
#include "Audio.h"
#include "Input.h"
#include "Render.h"
#include "SceneGame.h"
#include "EntityManager.h"
#include "Log.h"
#include "Point.h"

Coin::Coin(uint32 id, SDL_Rect bounds) : Entity(EntityType::COIN)
{
	name.Create("Coin");
	active = true;
	dim = app->entityManager->coinProperties.dim;
	SDL_Rect rec = { bounds.x,bounds.y,bounds.w,bounds.h };
	col = app->collisions->AddCollider(rec, Collider::Type::PICKUP, app->entityManager, this);

	this->pos = fPoint(bounds.x, bounds.y);
}

Coin::~Coin() {
	CleanUp();
}


bool Coin::Update(float dt)
{
	

	return true;
}

bool Coin::Draw(Render* render)
{
	//L13: TODO 3: Draw the PickUp
	app->render->DrawCircle((int)pos.x, (int)pos.y, dim, 199, 128, 241, 255, true);
	SDL_Rect rec = col->rect;
	app->render->DrawRectangle(rec, 249, 219, 73, 255, true, true);
	return true;
	return true;
}

bool Coin::CleanUp()
{
	if (col!=NULL)
		app->collisions->RemoveCollider(col);
	return true;
}

void Coin::OnCollision(Collider* c1, Collider* c2)
{
	if (c2->type == Collider::Type::PLAYER)
	{
		// sumar puntuaciones y lo q seqa mas
		toDestroy = true;
	}
}