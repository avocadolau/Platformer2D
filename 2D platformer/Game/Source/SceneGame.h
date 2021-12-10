#ifndef __SCENE_GAME_H__
#define __SCENE_GAME_H__

#include "Module.h"
#include "List.h"
#include "Platform.h"
#include "Map.h"

struct SDL_Texture;

class SceneGame : public Module
{
public:

	SceneGame();
	virtual ~SceneGame();

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


private:

	bool mapLoaded = false;
	SDL_Texture* img;
	SString backgroundPath;

	float parallax;
};

#endif // __SCENE_H__