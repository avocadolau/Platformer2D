#ifndef __SCENE_ELEMENTS_H__
#define	__SCENE_ELEMENTS_H__

#include "Module.h"
#include "Collider.h"
#include "Collisions.h"

#include "Platform.h"

class SceneElements : public Module
{
public:
	SceneElements();
	virtual ~SceneElements();

	bool Awake(pugi::xml_node& config);
	bool Start();
	bool PreUpdate();
	bool Update(float dt);
	bool PostUpdate();

	bool CleanUp();

	void DrawPaths();

	void AdFlyingEnemy();
	void AdWalkingEnemy();
	void AddPickUp();

public:
	List<Platform*> platforms;

private:
	SDL_Texture* platformImg;
	SString platformPath;
};

#endif
