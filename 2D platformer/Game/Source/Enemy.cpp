#include "Enemy.h"

Enemy::Enemy()
{

}

Enemy::~Enemy()
{

}

bool Enemy::Awake()
{

	return true;
}

bool Enemy::Start()
{

	return true;
}

bool Enemy::PreUpdate()
{

	return true;
}

bool Enemy::Update(float dt)
{

	return true;
}

bool Enemy::PostUpdate()
{
	return true;
}

bool Enemy::CleanUp()
{
	return true;
}

bool Enemy::LoadState(pugi::xml_node&)
{
	return true;
}

bool Enemy::SaveState(pugi::xml_node&) const
{
	return true;
}

void Enemy::OnCollision(Collider* c1, Collider* c2)
{

}