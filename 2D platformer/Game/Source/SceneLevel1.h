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

	bool Awake();
	bool Start();
	bool PreUpdate();
	bool Update(float dt);
	bool PostUpdate();
	
	// Called before quitting
	bool CleanUp();

public:
	List<Platform*> platforms;
	SDL_Texture* platformImg;
	

private:

	bool mapLoaded = false;
	SDL_Texture* img;
	Collider* ground;
	Collider* none;
	

};

#endif // __SCENE_H__