#ifndef __SCENE_H__
#define __SCENE_H__

#include "Module.h"
#include "List.h"
#include "Platform.h"

struct SDL_Texture;

class SceneLevel1 : public Module
{
public:

	SceneLevel1();
	virtual ~SceneLevel1();

	bool Awake(pugi::xml_node& config);
	bool Start();
	bool PreUpdate();
	bool Update(float dt);
	bool PostUpdate();
	
	bool CreateCollisions();
	bool CleanUp();

public:
	List<Platform*> platforms;
	SDL_Texture* platformImg;
	SDL_Texture* background;

	Collider* winCol = nullptr;
	Collider* borders = nullptr;

private:

	bool mapLoaded = false;
	SDL_Texture* img;
	SString platformPath;
	SString backgroundPath;

};

#endif // __SCENE_H__