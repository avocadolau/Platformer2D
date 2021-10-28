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

	gravity = 0.00015f;
	
	vel = { 0.05f,0 };
	maxVel = { 0.05f,0.2f };
	pos.x = 50;
	pos.y = 1408;

		
	SDL_Rect rec;



	//up
	rec = { 5,-1,dim.x - 8,1 };
	colUp = app->collisions->AddCollider(rec, Collider::Type::LISTENER, this);

	//down
	rec = { 5,dim.y,dim.x - 8,1 };
	colDown = app->collisions->AddCollider(rec, Collider::Type::LISTENER, this);

	//left
	rec = { -1,1,3,dim.y-2 };
	colLeft = app->collisions->AddCollider(rec, Collider::Type::LISTENER, this);

	//right
	rec = { dim.x-4,1,5,dim.y-2 };
	colRight = app->collisions->AddCollider(rec, Collider::Type::LISTENER, this);


	rec = { 0,0,dim.x,dim.y };
	collider = app->collisions->AddCollider(rec, Collider::Type::PLAYER, this);

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
	
	

	// vertical movement

	if (jumps > 0)
		if (app->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN)
		{
			pos.y--;
			jumps--;
			vel.y = -maxVel.y;
			down = true;
		}

	if (down == true)
	{
		vel.y += gravity * dt;
		if (vel.y > maxVel.y) vel.y = maxVel.y;
		if (vel.y < -maxVel.y)vel.y = -maxVel.y;
		pos.y += vel.y * dt;
	}
	

	// horizontal movement
	if (app->input->GetKey(SDL_SCANCODE_LEFT) == KEY_REPEAT && left == true)
		pos.x -= vel.x * dt;

	if (app->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_REPEAT && right == true)
		pos.x += vel.x * dt;



	colUp->SetPos((int)pos.x+4, (int)pos.y - 1);
	colDown->SetPos((int)pos.x+4, (int)pos.y + dim.y);
	colLeft->SetPos((int)pos.x - 1, (int)pos.y+1);
	colRight->SetPos((int)pos.x + dim.x-4, (int)pos.y+1);
	collider->SetPos((int)pos.x, (int)pos.y);

	camPos.x = (0 - pos.x) + 640;
	camPos.y = (0 - pos.y) + 360;

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
	if (c1 == colDown)
	{
		down == true;
		if (c2->type == Collider::Type::GROUND)
		{
			down = false;
			jumps = 2;
			vel.y = 0;

			pos.y = c2->rect.y - dim.y;
			colUp->rect.y = pos.y--;
			colDown->rect.y = pos.y + dim.y;
			colRight->rect.y = pos.y + 1;
			colLeft->rect.y = pos.y + 1;
		}
	}

	if (c1==colUp)
	{
		up = true;
		if (c2->type == Collider::Type::GROUND)
		{
			
			vel.y = 0;

			pos.y = (c2->rect.y + c2->rect.h)+2;
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
			colRight->rect.x = pos.x + dim.x-4;
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


}