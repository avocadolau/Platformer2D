#ifndef __ENTITY_PROPERTIES_H__
#define __ENTITY_PROPERTIES_H__

#include "Point.h"
#include "SString.h"
#include "Animation.h"

#include "Textures.h"

struct PlayerProperties
{
	int lives;
	float gravity;
	fPoint maxVel;
	iPoint dim;
	SString spritePath;
	
	float animSpeed;

	int idleW, idleH, idleN;
	int runW, runH, runN;
	int jumpW, jumpH, jumpN;
	int deathW, deathH, deathN;
};

struct SceneElements {
	SString platformPath;
	SDL_Texture* platformImg;
};

struct CoinProperties {
	int dim;
	SString path;
	SDL_Texture* sprite;
};

struct FlyEnemyProperties
{

};
#endif