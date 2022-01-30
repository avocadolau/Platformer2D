#ifndef __SCENE_GAME_H__
#define __SCENE_GAME_H__

#include "Module.h"
#include "List.h"
#include "Map.h"
#include "Enemy.h"
#include "Platform.h"
#include "PlayerAtack.h"
#include "Coin.h"

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
	bool RemoveGroundColliders();
	bool CleanUp();

	void OnCollision(Collider* c1, Collider* c2);

	bool LoadState(pugi::xml_node&);
	bool SaveState(pugi::xml_node&) const;

	bool OnGuiMouseClickEvent(GuiControl* control);

public:

	SDL_Texture* background;

	Collider* winCol;
	Collider* borders;

	Map* currentMap = nullptr;

	EnemyInfo flyInfo;
	EnemyInfo walkInfo;
	List<int*> deadEnemies;

	bool destroyDeadEnemies = false;
	bool pause = false;
	int highScore = 0;
	int level;
	float timer = 0;
	bool saved = false;

	int maxScoreLvl1, maxScoreLvl2;

private:

	bool mapLoaded = false;
	bool mainMenu = false;
	SDL_Texture* img;
	SString backgroundPath;
	SDL_Texture* platformImg;
	SString platformPath;

	float parallax;
};

#endif // __SCENE_H__