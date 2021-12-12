#ifndef __SCENE_GAME_H__
#define __SCENE_GAME_H__

#include "Module.h"
#include "List.h"
#include "Map.h"
#include "Enemy.h"
#include "Platform.h"

#include "SDL_image/include/SDL_image.h"

struct SDL_Texture;

struct EnemyInfo
{
	SString spritePath;
	SDL_Texture* spriteSheet;
	Animation anim, death;
};

class SceneGame : public Module
{
public:

	SceneGame();
	~SceneGame();

	bool Awake(pugi::xml_node& config);
	bool Start();
	bool PreUpdate();
	bool Update(float dt);
	bool PostUpdate();
	
	bool ChangeMap();
	
	bool CreateCollisions();

	bool CleanUp();

public:

	SDL_Texture* background;

	Collider* winCol;
	Collider* borders;

	Map* currentMap = nullptr;
	
	List<Platform*> platforms;
	List<Collider*> groundColliders;
	List<Enemy*> enemies;

	EnemyInfo flyInfo;
	EnemyInfo walkInfo;
	

private:

	bool mapLoaded = false;
	SDL_Texture* img;
	SString backgroundPath;
	SDL_Texture* platformImg;
	SString platformPath;

	float parallax;

};

#endif // __SCENE_H__