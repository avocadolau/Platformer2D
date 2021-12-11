#include "Enemy.h"

#include "App.h"
#include "Map.h"
#include "Player.h"
#include "SceneGame.h"
#include "PathFinding.h"
#include "Collisions.h"
#include "Collider.h"
#include "Render.h"

#include "Point.h"

Enemy::Enemy(iPoint dim_ ,SDL_Rect pDetector, iPoint lim1_, iPoint lim2_, Type type_)
{
	name.Create("flyingEnemy");
	active = true;
	dim = dim_;
	lim1 = lim1_;
	lim2 = lim2_;
	type = type_;

	pos = { (float)lim1.x,(float)lim1.y };
	tileDim = app->sceneGame->currentMap->mapData.tileHeight;

	SDL_Rect rec = { (int)pos.x,(int)pos.y,dim.x,dim.y };
	col = app->collisions->AddCollider(rec, Collider::Type::ENEMY, this);
	rec = { (int)pos.x,(int)pos.y,dim.x,2 };
	up = app->collisions->AddCollider(rec, Collider::Type::ENEMY, this);
	rec = { (int)pos.x,(int)pos.y + dim.y - 2,dim.x,2 };
	down = app->collisions->AddCollider(rec, Collider::Type::ENEMY, this);
	detector = app->collisions->AddCollider(pDetector, Collider::Type::DETECTOR, this);

	//pasar tambien las texturas T_T
}

Enemy::~Enemy()
{

}

bool Enemy::Awake()
{
	bool ret = false;

	return ret;
}

bool Enemy::Start()
{
	

	return true;
}

bool Enemy::PreUpdate()
{
	bool ret = false;

	

	return ret;
}

bool Enemy::Update(float dt)
{
	bool ret = false;

	if (state == ALIVE)
	{
		if (type == WALK) Walk(dt);
		if (type == FLY) Fly(dt);

		SetPosition({ (int)pos.x, (int)pos.y });
	}

	if (state == DEATH)
	{
		death.mustFlip = currentAnim->mustFlip;
		if (currentAnim != &death) currentAnim = &death;
		if (currentAnim->HasFinished() == true)
		{
			active = false;
		}

	}

	currentAnim->Update();
	
	

	return ret;
}

bool Enemy::PostUpdate()
{
	bool ret = false;

	//app->render->DrawTexture(sprites, pos.x, pos.y, &currAnim->GetCurrentFrame(), 1.0f, 0, -1, NULL, currAnim->mustFlip);

	if (app->debug == true) DrawPath();

	return ret;
}

bool Enemy::CleanUp()
{
	bool ret = false;

	app->collisions->RemoveCollider(col);
	app->collisions->RemoveCollider(up);
	app->collisions->RemoveCollider(down);
	app->collisions->RemoveCollider(detector);


	return ret;
}

bool Enemy::DrawPath()
{
	bool ret = false;

	if (path != NULL) ret = true;
	Uint8 alpha = 150;
	for (uint i = 0; i < path->Count(); ++i)
	{
		iPoint pos = app->sceneGame->currentMap->MapToWorld(path->At(i)->x, path->At(i)->y);
		SDL_Rect rec = { pos.x,pos.y,64,64 };

		app->render->DrawRectangle(rec, 255, 181, 221, alpha, true, true); //PINK
		if (alpha > 15)alpha -= 5;
	}

	return ret;
}

void Enemy::OnCollision(Collider* c1, Collider* c2)
{
	if (c1 == detector)
	{
		pNear = true;
	}
	if (c1 == up && c2 == app->player->colDown)
	{
		state = DEATH;
	}
}

void Enemy::SetPosition(iPoint pos)
{
	col->rect.x = pos.x;
	col->rect.y = pos.y;
	up->rect.x = pos.x;
	up->rect.y = pos.y;
	down->rect.x = pos.x;
	down->rect.y = pos.y + dim.y - 2;
}


void Enemy::Fly(float dt)
{
	currentAnim = &idleAnim;
	int offset = 5;

	coord = app->sceneGame->currentMap->WorldToMap(pos.x + (dim.x / 2), pos.y + (dim.y / 2));

	if ((int)pos.x % tileDim > (tileDim / 2) - offset && (int)pos.y % tileDim < (tileDim / 2) + offset)
	{

		if (pNear == true)
		{
			SDL_Rect pRect = app->player->collider->rect;
			iPoint pCoord = app->sceneGame->currentMap->WorldToMap(app->player->pos.x + (app->player->dim.x / 2), app->player->pos.y + (app->player->pos.y / 2));

			if (detector->Intersects(pRect) && app->pathfinding->IsWalkable(pCoord))
			{
				if (app->pathfinding->CreatePath(coord, pCoord, false) > 0)
					path = app->pathfinding->GetLastPath();
			}
			else pNear = false;
		}

		else
		{
			if (coord == lastLim)
			{
				if (lastLim == lim1) lastLim = lim2;
				else lastLim = lim2;
			}

			if (app->pathfinding->CreatePath(coord, lastLim, false))
				path = app->pathfinding->GetLastPath();
		}

	}

	iPoint nextTile = { path->At(0)->x, path->At(0)->y };
	float vel = app->player->maxVel.x;


	// meterle el must flim en la animacion T_T

	if (nextTile.x < coord.x) pos.x -= vel * dt;
	if (nextTile.x > coord.x) pos.x += vel * dt;
	if (nextTile.y < coord.y) pos.y -= vel * dt;
	if (nextTile.y > coord.y) pos.y += vel * dt;
}

void Enemy::Walk(float dt)
{
	currentAnim = &idleAnim;
	int offset = 5;

	coord = app->sceneGame->currentMap->WorldToMap(pos.x + (dim.x / 2), pos.y + (dim.y / 2));

	if ((int)pos.x % tileDim > (tileDim / 2) - offset)
	{
		if (pNear == true)
		{
			SDL_Rect pRect = app->player->collider->rect;
			iPoint pCoord = app->sceneGame->currentMap->WorldToMap(app->player->pos.x + (app->player->dim.x / 2), app->player->pos.y + (app->player->pos.y / 2));

			if (detector->Intersects(pRect) && app->pathfinding->IsWalkable(pCoord))
			{
				if (app->pathfinding->CreatePath(coord, pCoord,true) > 0)
					path = app->pathfinding->GetLastPath();
			}
			else pNear = false;
		}

		else
		{
			if (coord == lastLim)
			{
				if (lastLim == lim1) lastLim = lim2;
				else lastLim = lim2;
			}

			if (app->pathfinding->CreatePath(coord, lastLim, false))
				path = app->pathfinding->GetLastPath();
		}

	}

	iPoint nextTile = { path->At(0)->x, path->At(0)->y };
	float vel = app->player->maxVel.x;


	// meterle el must flim en la animacion T_T

	if (nextTile.x < coord.x) pos.x -= vel * dt;
	if (nextTile.x > coord.x) pos.x += vel * dt;
}

bool Enemy::LoadState(pugi::xml_node& node)
{
	return true;
}

bool Enemy::SaveState(pugi::xml_node& node) const
{
	
	return true;
}