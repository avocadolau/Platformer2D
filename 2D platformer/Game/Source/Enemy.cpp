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

Enemy::Enemy(int id_, iPoint dim_ ,SDL_Rect pDetector, iPoint lim1_, iPoint lim2_, Type type_)
{
	id = id_;
	active = true;
	dim = dim_;
	lim1 = lim1_;
	lim2 = lim2_;
	type = type_;

	lastLim = lim2;

	tileDim = app->sceneGame->currentMap->mapData.tileHeight;

	iPoint ipos = app->sceneGame->currentMap->MapToWorld((float)lim1.x, (float)lim1.y);
	ipos.x = ipos.x + tileDim / 2 - dim.x / 2;
	ipos.y = ipos.y + tileDim / 2 - dim.y / 2;
	pos = { (float)ipos.x,(float)ipos.y };

	SDL_Rect rec = { (int)pos.x,(int)pos.y,dim.x,dim.y };
	col = app->collisions->AddCollider(rec, Collider::Type::ENEMY, this);
	rec = { (int)pos.x,(int)pos.y,dim.x,2 };
	up = app->collisions->AddCollider(rec, Collider::Type::ENEMY, this);
	rec = { (int)pos.x,(int)pos.y + dim.y - 2,dim.x,2 };
	down = app->collisions->AddCollider(rec, Collider::Type::ENEMY, this);
	detector = app->collisions->AddCollider(pDetector, Collider::Type::DETECTOR, this);

	vel = app->player->maxVel.x / 2;

	//pasar tambien las texturas T_T y animaciones
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

		SetPosition(pos);
	}

	/*if (state == DEATH)
	{
		death.mustFlip = currentAnim->mustFlip;
		if (currentAnim != &death) currentAnim = &death;
		if (currentAnim->HasFinished() == true)
		{
			active = false;
		}

	}*/

	//currentAnim->Update();
	
	

	return ret;
}

bool Enemy::PostUpdate()
{
	bool ret = false;

	//app->render->DrawTexture(sprites, pos.x, pos.y, &currAnim->GetCurrentFrame(), 1.0f, 0, -1, NULL, currAnim->mustFlip);
	SDL_Rect rect = { pos.x,pos.y,dim.x,dim.y };
	app->render->DrawRectangle(rect, 255, 181, 221, 200, true, true); //PINK


	if (app->debug == true) DrawPath();
	if (app->debug == true)
	{
		app->render->DrawRectangle(detector->rect, 255, 255, 255, 60, true, true); //WHITE
	}

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

	if (path != NULL)
	{
		ret = true;

		Uint8 alpha = 150;
		for (uint i = 0; i < path->Count(); ++i)
		{
			iPoint pos = app->sceneGame->currentMap->MapToWorld(path->At(i)->x, path->At(i)->y);
			SDL_Rect rec = { pos.x,pos.y,64,64 };

			app->render->DrawRectangle(rec, 255, 181, 221, alpha, true, true); //PINK
			if (alpha > 15)alpha -= 5;
		}
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

void Enemy::SetPosition(fPoint pos)
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
	//currentAnim = &idleAnim;
	int offset = 20;

	coord = app->sceneGame->currentMap->WorldToMap((int)(pos.x + (dim.x / 2)), (int)(pos.y + (dim.y / 2)));

	if (coord == lastLim)
	{
		if (lastLim == lim1) lastLim = lim2;
		else if (lastLim == lim2) lastLim = lim1;
	}

	if ((int)pos.x % tileDim > (tileDim / 2) - offset && (int)pos.x % tileDim < (tileDim / 2) + offset)
	if ((int)pos.y % tileDim > (tileDim / 2) - offset && (int)pos.y % tileDim < (tileDim / 2) + offset)
	{
		if (pNear == true)
		{
			iPoint pPos = { (int)(app->player->pos.x + (app->player->dim.x / 2)) ,(int)(app->player->pos.y + (app->player->dim.y / 2))};
       		iPoint pCoord = app->sceneGame->currentMap->WorldToMap(pPos.x, pPos.y);

			if (pPos.x > detector->rect.x && pPos.x<detector->rect.x + detector->rect.w
				&& pPos.y>detector->rect.y && pPos.y < detector->rect.y + detector->rect.h)
			{
				if (app->pathfinding->CreatePath(coord, pCoord, false) > 0)
					path = app->pathfinding->GetLastPath();
				else pNear = false;
			}
			else pNear = false;

		}

		if (pNear == false)
		{

			bool pathCreated = app->pathfinding->CreatePath(coord, lastLim, false);
			path = app->pathfinding->GetLastPath();

		}

		if (path != NULL)
		{
			if (path->Count() > 1)
			{
				iPoint nextTile;
				nextTile = { path->At(1)->x, path->At(1)->y };
				if (nextTile.y < coord.y) dir = UP;
				if (nextTile.y > coord.y) dir = DOWN;
				if (nextTile.x < coord.x) dir = LEFT;
				if (nextTile.x > coord.x) dir = RIGHT;
			}
		}

		
			
		//else nextTile = { path->At(0)->x, path->At(0)->y };

		

	}

	if (path != NULL)
	{
		// meterle el must flim en la animacion T_T

		switch (dir)
		{
		case Enemy::UP:
			pos.y -= vel * dt;
			break;
		case Enemy::DOWN:
			pos.y += vel * dt;
			break;
		case Enemy::LEFT:
			pos.x -= vel * dt;
			break;
		case Enemy::RIGHT:
			pos.x += vel * dt;
			break;
		default:
			break;
		}
		SetPosition(pos);
	}
	
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

	iPoint nextTile = app->sceneGame->currentMap->MapToWorld(path->At(0)->x, path->At(0)->y);
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