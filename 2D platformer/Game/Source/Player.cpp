#include "Player.h"
#include "App.h"
#include "Input.h"
#include "Textures.h"
#include "Audio.h"
#include "Render.h"
#include "Window.h"
#include "SceneLevel1.h"
#include "Collisions.h"
#include "Collider.h"
#include "Map.h"
#include "Animation.h"

#include "Defs.h"
#include "Log.h"

Player::Player() : Module()
{
	name.Create("player");
	active = false;
}

Player::~Player()
{

}

bool Player::Awake()
{
	LOG("Loading Player");
	bool ret = true;

	return ret;
}

bool Player::Start()
{
	active = false;

	gravity = 0.00005f;
	
	vel = { 0.05f,0 };
	maxVel = { 0.05f,0.1f };
	pos.x = 50;
	pos.y = 300;
	camPos.x = app->render->camera.x;
	camPos.y = app->render->camera.y;

	grounded = false;
	
	SDL_Rect rec = { (int)pos.x,(int)pos.y,64,64 };
	SDL_Rect feetOfset = { (float)pos.x + 1,(int)pos.y + 64,64-2,1 };
	lastCol = nullptr;

	collider = app->collisions->AddCollider(rec, Collider::Type::PLAYER, this);
	feet = app->collisions->AddCollider(feetOfset, Collider::Type::LISTENER, this);

	app->collisions->debug = true;


	//----------------------------------------------------------------------------------
	sprites = app->tex->Load("Assets/Textures/playerSprites.png");
	
	for (int i = 0; i < 20; i++)
		idle.PushBack({ 35*i,0,35,64 });
	idle.loop = true;
	idle.mustFlip = false;
	idle.speed = 0.1f;

	for (int i = 0; i < 20; i++)
		run.PushBack({ 38 * i,64,38,64 });
	run.loop = true;
	run.mustFlip = false;
	run.speed = 0.1f;

	for (int i = 0; i < 20; i++)
		jump.PushBack({ 37 * i,128,37,64 });
	run.loop = false;
	run.mustFlip = false;
	run.speed = 0.1f;


	// load assets
	

	return true;
}

bool Player::PreUpdate()
{
	return true;
}

bool Player::Update(float dt)
{
	app->render->DrawTexture(sprites, 0, 500, NULL);

	// pues todods los controles que menuda pereza
	if (grounded == true)
		if (feet->Intersects(lastCol->rect) == false) grounded = false;
	

	// vertical movement

	if (jumps > 0)
		if (app->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN)
		{
			pos.y--;
			jumps--;
			vel.y = -maxVel.y;
			grounded = false;
		}

	if (grounded == false)
	{
		vel.y += gravity * dt;
		if (vel.y > maxVel.y) vel.y = maxVel.y;
		if (vel.y < -maxVel.y)vel.y = -maxVel.y;
		pos.y += vel.y * dt;
		camPos.y -= vel.y * dt;
	}
	

	// horizontal movement
	if (app->input->GetKey(SDL_SCANCODE_LEFT) == KEY_REPEAT)
	{
		pos.x -= vel.x * dt;
		camPos.x += vel.x * dt;
	}
	if (app->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_REPEAT)
	{
		pos.x += vel.x * dt;
		camPos.x -= vel.x * dt;
	}

	collider->rect.x = pos.x;
	collider->rect.y = pos.y;
	feet->rect.x = collider->rect.x + 1;
	feet->rect.y = collider->rect.y + 50;
	app->render->camera.x = camPos.x;
	app->render->camera.y = camPos.y;



	return true;
}

bool Player::PostUpdate()
{

	if (currentAnim == nullptr) currentAnim = &idle;
	switch (state)
	{
	case IDLE:
		currentAnim = &idle;
		break;
	case RUN:
		currentAnim = &run;
		break;
	case JUMP:
		currentAnim = &jump;
		break;
	case DEATH:
		currentAnim = &death;
		break;
	default:
		break;
	}

	

	return true;
}

bool Player::CleanUp()
{

	return true;
}

bool Player::LoadState(pugi::xml_node&)
{

	return true;
}

bool Player::SaveState(pugi::xml_node&)
{

	return true;
}

void Player::OnCollision(Collider* c1, Collider* c2)
{
	if (c1->type == Collider::Type::LISTENER)
	{
		grounded = true;
		jumps = 2;
		vel.y = 0;
		lastCol = c2;

		c1->rect.y = c2->rect.y;
		collider->rect.y = c1->rect.y - 50;
		pos.y = c1->rect.y - 50;
	}

	/*if (c1->type == Collider::Type::PLAYER)
	{
		if (c2->type == Collider::Type::GROUND)
		{
			if (grounded==false) while (c1->Intersects(c2->rect))
			{
				if (c1->rect.x < c2->rect.x && c1->rect.x + c1->rect.w > c2->rect.x)
				{
					c1->rect.x--;
					pos.x = c1->rect.x;
					feet->rect.x = c1->rect.x + 1;
					camPos.x += 1;
				}
				if (c1->rect.x > c2->rect.x && c1->rect.x < c2->rect.x + c2->rect.w)
				{
					c1->rect.x++;
					pos.x = c1->rect.x;
					feet->rect.x = c1->rect.x + 1;
					camPos.x += 1;
				}
			}
			
		}


	}*/
	



}