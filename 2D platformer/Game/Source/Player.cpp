#include "Player.h"
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

	CreateColliders();
	
	currentAnim = &idle;
	
	return true;
}

bool Player::PreUpdate()
{
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

	currentAnim->Update();

	return true;
}

bool Player::PostUpdate()
{
	app->render->DrawTexture(sprites, pos.x, pos.y, &currentAnim->GetCurrentFrame(), 1.0f, 0, -1, NULL, currentAnim->mustFlip);

	if (currentAnim == nullptr) currentAnim = &idle;

	if (app->input->GetKey(SDL_SCANCODE_F10) == KEY_DOWN) godMode = !godMode;

	return true;
}

bool Player::CleanUp()
{

	return true;
}

bool Player::LoadState(pugi::xml_node& node)
{
	level = node.attribute("level").as_int();
	pos.x = node.child("position").attribute("x").as_float();
	pos.y = node.child("position").attribute("y").as_float();

	app->sceneGame->ChangeMap();
	return true;
}

bool Player::SaveState(pugi::xml_node& node) const
{
	pugi::xml_node position = node.append_child("position");
	position.append_attribute("x").set_value(pos.x);
	position.append_attribute("y").set_value(pos.y);
	node.append_attribute("level").set_value(level);

	
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
	rec = { 5,-1,dim.x - 8,1 };
	colUp = app->collisions->AddCollider(rec, Collider::Type::PLAYER, this);
	//down
	rec = { 5,dim.y,dim.x - 8,1 };
	colDown = app->collisions->AddCollider(rec, Collider::Type::PLAYER, this);
	//left
	rec = { -1,1,3,dim.y - 2 };
	colLeft = app->collisions->AddCollider(rec, Collider::Type::PLAYER, this);
	//right
	rec = { dim.x - 4,1,5,dim.y - 2 };
	colRight = app->collisions->AddCollider(rec, Collider::Type::PLAYER, this);
	//body
	rec = { 0,0,dim.x,dim.y };
	collider = app->collisions->AddCollider(rec, Collider::Type::PLAYER, this);




	return true;
}

void Player::OnCollision(Collider* c1, Collider* c2)
{
	if (c2==app->sceneGame->winCol)
	{
		if (level == 1)
		{
			level = 2;
			app->fade->Fade(app->sceneGame, app->sceneGame, app->fade->time / app->dt);
		}
		if(level==2)app->fade->Fade(app->sceneGame, app->sceneWin, app->fade->time / app->dt);
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