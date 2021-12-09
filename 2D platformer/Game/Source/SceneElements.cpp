#include "Log.h"

#include "SceneElements.h"


SceneElements::SceneElements() :Module()
{
	name.Create("elements");

	
}

SceneElements::~SceneElements()
{


}
bool SceneElements::Awake(pugi::xml_node& config)
{
	LOG("Loading Scene Elements");
	bool ret = true;

	platformPath = config.attribute("platformPath").as_string();

	return ret;
}


bool SceneElements::Start()
{

	platformImg = app->tex->Load(platformPath.GetString());
	return true;
}

bool SceneElements::PreUpdate()
{



	return true;
}

bool SceneElements::Update(float dt)
{
	ListItem<Platform*>* item = platforms.start;
	while (item != NULL)
	{
		item->data->Update(dt);
		app->render->DrawTexture(platformImg, item->data->pos.x, item->data->pos.y, NULL);

		item = item->next;
	}

	return true;
}

bool SceneElements::PostUpdate()
{
	
	return true;
}

bool SceneElements::CleanUp()
{

	platforms.clear();

	return true;
}

