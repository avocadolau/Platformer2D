#include "Player.h"
#include "Entity.h"
#include "EntityManager.h"
#include "App.h"
#include "Input.h"
#include "Textures.h"
#include "Audio.h"
#include "Render.h"
#include "Window.h"
#include "SceneGame.h"
#include "SceneWin.h"
#include "SceneLose.h"
#include "Collisions.h"
#include "Collider.h"
#include "Map.h"
#include "Animation.h"
#include "FadeToBlack.h"

#include "Defs.h"
#include "Log.h"

Player::Player() : Entity(EntityType::PLAYER)
{
	name.Create("player");
	active = false;
}

Player::~Player()
{
	CleanUp();
}

bool Player::Awake(pugi::xml_node& config)
{
	LOG("Loading Player");
	bool ret = true;

	lives = config.attribute("lives").as_int();
	maxVel.x = config.child("physics").attribute("maxVelX").as_float();
	maxVel.y = config.child("physics").attribute("maxVelY").as_float();
	gravity = config.child("physics").attribute("gravity").as_float();
	dim.x = config.child("dimensions").attribute("x").as_int();
	dim.y = config.child("dimensions").attribute("y").as_int();
	
	animSpeed = config.child("animations").attribute("speed").as_float();
	
	int width, height, sprites;

	width = config.child("animations").child("idle").attribute("width").as_int();
	height= config.child("animations").child("idle").attribute("height").as_int();
	sprites = config.child("animations").child("idle").attribute("sprites").as_int();

	for (int i = 0; i < sprites; i++)
		idle.PushBack({ width * i,0,width,height });
	idle.loop = true;
	idle.mustFlip = false;
	idle.speed = animSpeed;

	width = config.child("animations").child("run").attribute("width").as_int();
	height = config.child("animations").child("run").attribute("height").as_int();
	sprites = config.child("animations").child("run").attribute("sprites").as_int();

	for (int i = 0; i < sprites; i++)
		run.PushBack({ width * i,height * 1,width,height });
	run.loop = true;
	run.mustFlip = false;
	run.speed = animSpeed;

	width = config.child("animations").child("jump").attribute("width").as_int();
	height = config.child("animations").child("jump").attribute("height").as_int();
	sprites = config.child("animations").child("jump").attribute("sprites").as_int();

	for (int i = 0; i < sprites; i++)
		jump.PushBack({ width * i,height * 2,width,height });
	jump.loop = true;
	jump.mustFlip = false;
	jump.speed = animSpeed;

	width = config.child("animations").child("death").attribute("width").as_int();
	height = config.child("animations").child("death").attribute("height").as_int();
	sprites = config.child("animations").child("death").attribute("sprites").as_int();

	for (int i = 0; i < sprites; i++)
		death.PushBack({ width * i,height * 3,width,height });
	death.loop = false;
	death.mustFlip = false;
	death.speed = animSpeed;

	
	return ret;
}

bool Player::Start()
{
	active = false;

	lives = app->entityManager->playerProperties.lives;
	maxVel.x = app->entityManager->playerProperties.maxVel.x;
	maxVel.y = app->entityManager->playerProperties.maxVel.y;
	gravity = app->entityManager->playerProperties.gravity;
	dim.x = app->entityManager->playerProperties.dim.x;
	dim.y = app->entityManager->playerProperties.dim.y;

	animSpeed = app->entityManager->playerProperties.animSpeed;
	sprites = app->tex->Load(app->entityManager->playerProperties.spritePath.GetString());

	int width, height, sprites;

	width = app->entityManager->playerProperties.idleW;
	height = app->entityManager->playerProperties.idleH;
	sprites = app->entityManager->playerProperties.idleN;

	for (int i = 0; i < sprites; i++)
		idle.PushBack({ width * i,0,width,height });
	idle.loop = true;
	idle.mustFlip = false;
	idle.speed = animSpeed;

	width = app->entityManager->playerProperties.runW;
	height = app->entityManager->playerProperties.runH;
	sprites = app->entityManager->playerProperties.runN;

	for (int i = 0; i < sprites; i++)
		run.PushBack({ width * i,height * 1,width,height });
	run.loop = true;
	run.mustFlip = false;
	run.speed = animSpeed;

	width = app->entityManager->playerProperties.jumpW;
	height = app->entityManager->playerProperties.jumpH;
	sprites = app->entityManager->playerProperties.jumpN;

	for (int i = 0; i < sprites; i++)
		jump.PushBack({ width * i,height * 2,width,height });
	jump.loop = true;
	jump.mustFlip = false;
	jump.speed = animSpeed;

	width = app->entityManager->playerProperties.deathW;
	height = app->entityManager->playerProperties.deathH;
	sprites = app->entityManager->playerProperties.deathN;

	for (int i = 0; i < sprites; i++)
		death.PushBack({ width * i,height * 3,width,height });
	death.loop = false;
	death.mustFlip = false;
	death.speed = animSpeed;

	vel = { maxVel.x,0 };

	CreateColliders();
	
	currentAnim = &idle;
	app->player = this;
	shotCoolDown.Start();

	return true;
}

bool Player::Update(float dt)
{
	// vertical movement
	if (alive == false)
	{
		death.mustFlip = currentAnim->mustFlip;
		if (currentAnim != &death) currentAnim = &death;
		if (currentAnim->HasFinished() == true)
		{
			app->player->lives--;
			app->fade->Fade(app->sceneGame, app->sceneLose, app->fade->time / dt);
		}
	}

	else
	{
		if (godMode == true)
		{
			
			currentAnim = &jump;

			// vertical movement
			if (app->input->GetKey(SDL_SCANCODE_W) == KEY_REPEAT && left == true) {
				pos.y -= vel.x * dt * 2;
			}

			if (app->input->GetKey(SDL_SCANCODE_S) == KEY_REPEAT && left == true) {
				pos.y += vel.x * dt * 2;
			}
			 
			// horizontal movement
			if (app->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT && left == true) {
				pos.x -= vel.x * dt*2;
				currentAnim->mustFlip = true;
			}

			if (app->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT && right == true) {
				pos.x += vel.x * dt*2;
				currentAnim->mustFlip = false;
			}
		}

		if (godMode == false)
		{
			if (down == false)
			{
				idle.mustFlip = currentAnim->mustFlip;
				currentAnim = &idle;
				if (colDown->Intersects(lastGround->rect) == false)
					down = true;

			}

			if (jumps > 0)
				if (app->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN)
				{
					app->audio->PlayFx(1,0);
					pos.y--;
					jumps--;
					vel.y = -maxVel.y;
					down = true;
				}

			if (down == true)
			{
				if (jumps == 2) jumps = 1;
				vel.y += gravity * dt;
				if (vel.y > maxVel.y) vel.y = maxVel.y;
				if (vel.y < -maxVel.y)vel.y = -maxVel.y;
				pos.y += vel.y * dt;
				jump.mustFlip = currentAnim->mustFlip;
				currentAnim = &jump;
			}

			// horizontal movement
			if (app->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT && left == true) {
				pos.x -= vel.x * dt;
				if (currentAnim == &idle) currentAnim = &run;
				currentAnim->mustFlip = true;
			}

			if (app->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT && right == true) {
				pos.x += vel.x * dt;
				if (currentAnim == &idle) currentAnim = &run;
				currentAnim->mustFlip = false;
			}
		}

		colUp->SetPos((int)pos.x + 4, (int)pos.y - 1);
		colDown->SetPos((int)pos.x + 4, (int)pos.y + dim.y);
		colLeft->SetPos((int)pos.x - 1, (int)pos.y + 1);
		colRight->SetPos((int)pos.x + dim.x - 4, (int)pos.y + 1);
		collider->SetPos((int)pos.x, (int)pos.y);

		app->render->camera.x = (int)-pos.x + (app->win->GetWidth() / 2);
		app->render->camera.y = (int)-pos.y + (app->win->GetHeight() / 2);

		up = true;
		down = true;
		right = true;
		left = true;
	}

	if (app->input->GetKey(SDL_SCANCODE_RETURN) == KEY_DOWN)
	{
		if (shotCoolDown.Read() >= COOLDOWN)
		{
			float velocity = vel.x;
			if (currentAnim->mustFlip == true) velocity = 0 - velocity;
			// arreglar
			app->entityManager->CreateEntity(EntityType::ATACK, 0, { (int)pos.x, (int)pos.y,0,0 });
			shotCoolDown.Start();
		}

	}

	currentAnim->Update();

	return true;
}

bool Player::Draw(Render* render)
{
	if (currentAnim == nullptr) currentAnim = &idle;

	return app->render->DrawTexture(sprites, pos.x, pos.y, &currentAnim->GetCurrentFrame(), 1.0f, 0, -1, NULL, currentAnim->mustFlip);
}


bool Player::CleanUp()
{
	return true;
}

bool Player::Load(pugi::xml_node& node)
{
	pos.x = node.child("position").attribute("x").as_float();
	pos.y = node.child("position").attribute("y").as_float();

	return true;
}

bool Player::Save(pugi::xml_node& node) const
{
	pugi::xml_node position = node.append_child("position");
	position.append_attribute("x").set_value(pos.x);
	position.append_attribute("y").set_value(pos.y);

	return true;
}

bool Player::CreateColliders()
{
	SDL_Rect rec;

	if (colUp != NULL) app->collisions->RemoveCollider(colUp);
	if (colDown != NULL) app->collisions->RemoveCollider(colDown);
	if (colLeft != NULL) app->collisions->RemoveCollider(colLeft);
	if (colRight != NULL) app->collisions->RemoveCollider(colRight);
	if (collider != NULL) app->collisions->RemoveCollider(collider);


	//up
	rec = { (int)pos.x + 4,(int)pos.y - 1,dim.x - 8,1 };
	colUp = app->collisions->AddCollider(rec, Collider::Type::PLAYER, app->entityManager, this);
	//down
	rec = { (int)pos.x + 4,(int)pos.y + dim.y,dim.x - 8,1 };
	colDown = app->collisions->AddCollider(rec, Collider::Type::PLAYER, app->entityManager, this);
	//left
	rec = { (int)pos.x - 1,(int)pos.y + 1,3,dim.y - 2 };
	colLeft = app->collisions->AddCollider(rec, Collider::Type::PLAYER, app->entityManager, this);
	//right
	rec = { (int)pos.x + dim.x - 4,(int)pos.y + 1,5,dim.y - 2 };
	colRight = app->collisions->AddCollider(rec, Collider::Type::PLAYER, app->entityManager, this);
	//body
	rec = { (int)pos.x,(int)pos.y,dim.x,dim.y };
	collider = app->collisions->AddCollider(rec, Collider::Type::PLAYER, app->entityManager, this);

	return true;
}

void Player::OnCollision(Collider* c1, Collider* c2)
{
	if (c2==app->sceneGame->winCol)
	{
		if (app->sceneGame->level == 1)
		{
			app->sceneGame->level = 2;
			app->fade->Fade(app->sceneGame, app->sceneGame, app->fade->time / app->dt);
		}
		if(app->sceneGame->level==2)app->fade->Fade(app->sceneGame, app->sceneWin, app->fade->time / app->dt);
	}

	if (godMode == true)
	{
		if (c2 ==app->sceneGame->borders)
		{
			if (colDown->rect.y>c2->rect.h)
			{
				vel.y = -maxVel.y;
			}

			up = true;
			if (colUp->rect.y < c2->rect.y)
			{
				up = false;
			}

			if (colLeft->rect.x < c2->rect.x)
			{
				pos.x = c2->rect.x + 1;
				colUp->rect.x = pos.x + 4;
				colDown->rect.x = pos.x + 4;
				colLeft->rect.x = pos.x - 1;
				colRight->rect.x = pos.x + dim.x - 4;
			}

			if (colRight->rect.x + colRight->rect.w > c2->rect.w)
			{
				pos.x = c2->rect.w - dim.x -1;
				colUp->rect.x = pos.x + 4;
				colDown->rect.x = pos.x + 4;
				colLeft->rect.x = pos.x - 1;
				colRight->rect.x = pos.x + dim.x - 4;
			}
		}

	}
	
	else
	{
		if (app->sceneGame->borders)
			if (colDown->rect.y > app->sceneGame->borders->rect.h)
				alive = false;

		if (c1 == colDown)
		{
			down == true;
			if (c2->type == Collider::Type::GROUND)
			{
				down = false;
				jumps = 2;
				vel.y = 0;

				pos.y = c2->rect.y - dim.y + 1;
				colUp->rect.y = pos.y--;
				colDown->rect.y = pos.y + dim.y;
				colRight->rect.y = pos.y + 1;
				colLeft->rect.y = pos.y + 1;
				lastGround = c2;
			}
		}

		if (c1 == colUp)
		{
			up = true;
			if (c2->type == Collider::Type::GROUND)
			{

				vel.y = 0;

				pos.y = (c2->rect.y + c2->rect.h) + 2;
				colUp->rect.y = pos.y--;
				colDown->rect.y = pos.y + dim.y;
				colRight->rect.y = pos.y + 1;
				colLeft->rect.y = pos.y + 1;
			}
		}


		if (c1 == colLeft)
		{
			left = true;
			if (c2->type == Collider::Type::GROUND)
			{
				left = false;

				pos.x = c2->rect.x + c2->rect.w;
				colUp->rect.x = pos.x + 4;
				colDown->rect.x = pos.x + 4;
				colLeft->rect.x = pos.x - 1;
				colRight->rect.x = pos.x + dim.x - 4;
			}
		}

		if (c1 == colRight)
		{
			right = true;
			if (c2->type == Collider::Type::GROUND)
			{
				right = false;

				pos.x = c2->rect.x - dim.x;
				colUp->rect.x = pos.x + 4;
				colDown->rect.x = pos.x + 4;
				colLeft->rect.x = pos.x - 1;
				colRight->rect.x = pos.x + dim.x - 4;
			}
		}

		app->render->camera.x = (int)-pos.x + (app->win->GetWidth() / 2);
		app->render->camera.y = (int)-pos.y + (app->win->GetHeight() / 2);
	}

}

void Player::Enable()
{
	active = true;
	alive = true;
	CreateColliders();
}

void Player::Disable()
{
	active = false;

	if (colUp != NULL) app->collisions->RemoveCollider(colUp);
	if (colDown != NULL) app->collisions->RemoveCollider(colDown);
	if (colLeft != NULL) app->collisions->RemoveCollider(colLeft);
	if (colRight != NULL) app->collisions->RemoveCollider(colRight);
	if (collider != NULL) app->collisions->RemoveCollider(collider);
}