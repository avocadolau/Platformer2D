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

bool Player::Awake(pugi::xml_node& config)
{
	LOG("Loading Player");
	bool ret = true;

	maxVel.x = config.child("physics").attribute("maxVelX").as_float();
	maxVel.y = config.child("physics").attribute("maxVelY").as_float();
	gravity = config.child("physics").attribute("gravity").as_float();
	dim.x = config.child("dimensions").attribute("x").as_int();
	dim.y = config.child("dimensions").attribute("y").as_int();
	
	animSpeed = config.child("animations").attribute("speed").as_float();
	spritesPath=("%s", config.child("animations").attribute("path").as_string());
	
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

	vel = { maxVel.x,0 };

	sprites = app->tex->Load(spritesPath.GetString());

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
	//body
	rec = { 0,0,dim.x,dim.y };
	collider = app->collisions->AddCollider(rec, Collider::Type::PLAYER, this);

	currentAnim = &idle;
	
	return true;
}

bool Player::PreUpdate()
{
	return true;
}

bool Player::Update(float dt)
{

	// pues todods los controles que menuda pereza

	if (down == false)
	{
		currentAnim = &idle;
		if (colDown->Intersects(lastGround->rect) == false)
			down = true;

	}
	

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
		currentAnim = &jump;
	}
	

	// horizontal movement
	if (app->input->GetKey(SDL_SCANCODE_LEFT) == KEY_REPEAT && left == true) {
		pos.x -= vel.x * dt;
		if (currentAnim == &idle) currentAnim = &run;
		currentAnim->mustFlip = true;
	}

	if (app->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_REPEAT && right == true) {
		pos.x += vel.x * dt;
		if (currentAnim == &idle) currentAnim = &run;
		currentAnim->mustFlip = false;
	}


	colUp->SetPos((int)pos.x+4, (int)pos.y - 1);
	colDown->SetPos((int)pos.x+4, (int)pos.y + dim.y);
	colLeft->SetPos((int)pos.x - 1, (int)pos.y+1);
	colRight->SetPos((int)pos.x + dim.x-4, (int)pos.y+1);
	collider->SetPos((int)pos.x, (int)pos.y);

	app->render->camera.x = (int)-pos.x+(app->win->GetWidth()/2);
	app->render->camera.y = (int)-pos.y+(app->win->GetHeight()/2);

	
	currentAnim->Update();

	return true;
}

bool Player::PostUpdate()
{

	if (currentAnim->mustFlip == false)app->render->DrawTexture(sprites, pos.x, pos.y, &currentAnim->GetCurrentFrame());
	else
	{

	}
	bool res= app->render->DrawTexture(sprites, pos.x, pos.y, &currentAnim->GetCurrentFrame(),1.0F,0,-1,NULL);


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

			pos.y = c2->rect.y - dim.y+1;
			colUp->rect.y = pos.y--;
			colDown->rect.y = pos.y + dim.y;
			colRight->rect.y = pos.y + 1;
			colLeft->rect.y = pos.y + 1;
			lastGround = c2;
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