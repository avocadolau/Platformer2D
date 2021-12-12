#include "Checkpoint.h"
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

Checkpoint::Checkpoint() : Module()
{
	name.Create("checkpoint");
	active = false;
}

Checkpoint::~Checkpoint()
{

}

bool Checkpoint::Awake(pugi::xml_node& config)
{
	LOG("Loading Player");
	bool ret = true;

	dim.x = 0;
	dim.y = 0;
	dim.w = config.child("sprite").attribute("width").as_int();
	dim.h= config.child("sprite").attribute("height").as_int();
	spritesPath = config.child("sprite").attribute("path").as_string();

	pos.x = config.child("state").attribute("x").as_int();
	pos.y = config.child("state").attribute("y").as_int();

	activated = config.child("state").attribute("activated").as_bool();

	return ret;
}

bool Checkpoint::Start()
{
	active = false;

	sprites = app->tex->Load(spritesPath.GetString());

	CreateColliders();
	return true;
}

bool Checkpoint::PreUpdate()
{
	return true;
}

bool Checkpoint::Update(float dt)
{
	
	return true;
}

bool Checkpoint::PostUpdate()
{
	app->render->DrawTexture(sprites, pos.x, pos.y, &dim);

	return true;
}

bool Checkpoint::CleanUp()
{

	return true;
}

bool Checkpoint::LoadState(pugi::xml_node& node)
{
	

	return true;
}

bool Checkpoint::SaveState(pugi::xml_node& node) const
{
	

	
	return true;
}

bool Checkpoint::CreateColliders()
{
	if (collider != NULL) app->collisions->RemoveCollider(collider);

	collider = app->collisions->AddCollider(dim, Collider::Type::DETECTOR, this);

	return true;
}

void Checkpoint::OnCollision(Collider* c1, Collider* c2)
{
	if (activated) return;

	if (c2->type == Collider::Type::PLAYER) {
		activated = true;
		app->SaveGameRequest();
	}
}
