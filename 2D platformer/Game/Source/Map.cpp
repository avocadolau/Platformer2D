
#include "App.h"
#include "Render.h"
#include "Textures.h"
#include "Map.h"
#include "Collisions.h"
#include "Collider.h"
#include "Player.h"
#include "SceneGame.h"
#include "Platform.h"
#include "Defs.h"
#include "Log.h"
#include "EntityManager.h"
#include "Entity.h"
#include <math.h>

Map::Map() : Module(), mapLoaded(false)
{
	name.Create("map");
}

// Destructor
Map::~Map()
{}

// L06: TODO 7: Ask for the value of a custom property
int Properties::GetProperty(const char* value, int defaultValue) const
{
	ListItem<Property*>* item = list.start;

	while (item)
	{
		if (item->data->name == value)
			return item->data->value;
		item = item->next;
	}

	return defaultValue;
}

// Called before render is available
bool Map::Awake(pugi::xml_node& config)
{
    LOG("Loading Map Parser");
    bool ret = true;

    folder.Create(config.child("folder").child_value());

	for (pugi::xml_node enemyNode = config.child("enemies").child("enemy"); enemyNode && ret; enemyNode = enemyNode.next_sibling("enemy"))
	{
		ret = true;

		EnemyData* newEnemy = new EnemyData();

		newEnemy->id= enemyNode.attribute("id").as_int();

		if (enemyNode.attribute("type").as_string() == "fly") newEnemy->type = EntityType::FLY_ENEMY;
		if (enemyNode.attribute("type").as_string() == "walk") newEnemy->type = EntityType::WALK_ENEMY;

		newEnemy->dim= { enemyNode.attribute("w").as_int(),enemyNode.attribute("h").as_int() };

		newEnemy->detector= { enemyNode.child("detector").attribute("x").as_int(), enemyNode.child("detector").attribute("y").as_int(),
							enemyNode.child("detector").attribute("w").as_int(), enemyNode.child("detector").attribute("h").as_int() };

		newEnemy->lim1 = { enemyNode.child("lim1").attribute("x").as_int(),enemyNode.child("lim1").attribute("y").as_int() };
		newEnemy->lim2 = { enemyNode.child("lim2").attribute("x").as_int(),enemyNode.child("lim2").attribute("y").as_int() };


		enemyData.add(newEnemy);

	}

    return ret;
}

// Draw the map (all requried layers)
void Map::Draw()
{
	if (mapLoaded == false) return;


	// L04: DONE 5: Prepare the loop to draw all tilesets + DrawTexture()
	ListItem<MapLayer*>* mapLayerItem;

	// L06: TODO 4: Make sure we draw all the layers and not just the first one.
	for (int i = 1; i < mapData.layers.count(); i++)
	{
		mapLayerItem = mapData.layers.start;

		while (mapLayerItem != NULL)
		{
			if (mapLayerItem->data->properties.GetProperty("Draw") == i) {

				for (int x = 0; x < mapLayerItem->data->width; x++)
				{
					for (int y = 0; y < mapLayerItem->data->height; y++)
					{
						// L04: DONE 9: Complete the draw function
						int gid = mapLayerItem->data->Get(x, y);

						if (gid > 0) {

							TileSet* tileset = GetTilesetFromTileId(gid);

							SDL_Rect r = tileset->GetTileRect(gid);
							iPoint pos = MapToWorld(x, y);

							app->render->DrawTexture(tileset->texture,
								pos.x,
								pos.y,
								&r);
						}

					}
				}
			}

			mapLayerItem = mapLayerItem->next;
		}
	}
	
		
}

iPoint Map::MapToWorld(int x, int y) const
{
	iPoint ret;

	// L05: DONE 1: Add isometric map to world coordinates
	if (mapData.type == MAPTYPE_ORTHOGONAL)
	{
		ret.x = x * mapData.tileWidth;
		ret.y = y * mapData.tileHeight;
	}
	else if (mapData.type == MAPTYPE_ISOMETRIC)
	{
		ret.x = (x - y) * (mapData.tileWidth / 2);
		ret.y = (x + y) * (mapData.tileHeight / 2);
	}
	else
	{
		LOG("Unknown map type");
		ret.x = x; ret.y = y;
	}

	return ret;
}

iPoint Map::WorldToMap(int x, int y) const
{
	iPoint ret(0, 0);

	// L05: DONE 3: Add the case for isometric maps to WorldToMap
	if (mapData.type == MAPTYPE_ORTHOGONAL)
	{
		ret.x = x / mapData.tileWidth;
		ret.y = y / mapData.tileHeight;
	}
	else if (mapData.type == MAPTYPE_ISOMETRIC)
	{

		float half_width = mapData.tileWidth * 0.5f;
		float half_height = mapData.tileHeight * 0.5f;
		ret.x = int((x / half_width + y / half_height) / 2);
		ret.y = int((y / half_height - (x / half_width)) / 2);
	}
	else
	{
		LOG("Unknown map type");
		ret.x = x; ret.y = y;
	}

	return ret;
}

TileSet* Map::GetTilesetFromTileId(int id) const
{
	ListItem<TileSet*>* item = mapData.tilesets.start;
	TileSet* set = item->data;

	while (item)
	{
		if (id < item->data->firstgid)
		{
			set = item->prev->data;
			break;
		}
		set = item->data;
		item = item->next;
	}

	return set;
}

SDL_Rect TileSet::GetTileRect(int id) const
{
	SDL_Rect rect = { 0 };

	// L04: DONE 7: Get relative Tile rectangle
	int relativeId = id - firstgid;
	rect.w = tileWidth;
	rect.h = tileHeight;

	rect.x = margin + ((rect.w + spacing) * (relativeId % columns));
	rect.y = margin + ((rect.h + spacing) * (relativeId / columns));
	
	return rect;
}

bool Map::CleanUp()
{
    LOG("Unloading map");

    // L03: DONE 2: Make sure you clean up any memory allocated from tilesets/map
    // Remove all tilesets
	ListItem<TileSet*>* item;
	item = mapData.tilesets.start;

	while (item != NULL)
	{
		RELEASE(item->data);
		item = item->next;
	}
	mapData.tilesets.clear();

	// L04: DONE 2: clean up all layer data
	// Remove all layers
	ListItem<MapLayer*>* item2;
	item2 = mapData.layers.start;

	while (item2 != NULL)
	{
		RELEASE(item2->data);
		item2 = item2->next;
	}
	mapData.layers.clear();


	if (walkMap != NULL) RELEASE_ARRAY(walkMap);
	if (walkMap != NULL) RELEASE_ARRAY(flyMap);

    return true;
}

bool Map::Load(const char* filename)
{
    bool ret = true;
    SString tmp("%s%s", folder.GetString(), filename);

	pugi::xml_document mapFile; 
    pugi::xml_parse_result result = mapFile.load_file(tmp.GetString());

    if(result == NULL)
    {
        LOG("Could not load map xml file %s. pugi error: %s", filename, result.description());
        ret = false;
    }

	// Load general info
    if(ret == true)
    {
        // L03: DONE 3: Create and call a private function to load and fill all your map data
		ret = LoadMap(mapFile);
	}

    // L03: DONE 4: Create and call a private function to load a tileset
    // remember to support more any number of tilesets!
	if (ret == true)
	{
		ret = LoadTileSets(mapFile.child("map"));
	}

	// L04: DONE 4: Iterate all layers and load each of them
	// Load layer info
	if (ret == true)
	{
		ret = LoadAllLayers(mapFile.child("map"));
		CreateWalkabilityMap(&walkMap);
		CreateFlyabilityMap(&flyMap);
	}


    if(ret == true)
    {
        // L03: TODO 5: LOG all the data loaded iterate all tilesets and LOG everything

		// L04: TODO 4: LOG the info for each loaded layer
    }

	

    mapLoaded = ret;

    return ret;
}

bool Map::LoadMap(pugi::xml_node mapFile)
{
	bool ret = true;
	pugi::xml_node map = mapFile.child("map");

	if (map == NULL)
	{
		LOG("Error parsing map xml file: Cannot find 'map' tag.");
		ret = false;
	}
	else
	{
		// L03: DONE 3: Load map general properties
		mapData.height = map.attribute("height").as_int();
		mapData.width = map.attribute("width").as_int();
		mapData.tileHeight = map.attribute("tileheight").as_int();
		mapData.tileWidth = map.attribute("tilewidth").as_int();

		LoadMapProperties(map, mapData.properties);

		// L05: DONE 1: Add formula to go from isometric map to world coordinates
		mapData.type = MAPTYPE_UNKNOWN;
		if (strcmp(map.attribute("orientation").as_string(), "isometric") == 0)
		{
			mapData.type = MAPTYPE_ISOMETRIC;
		}
		if (strcmp(map.attribute("orientation").as_string(), "orthogonal") == 0)
		{
			mapData.type = MAPTYPE_ORTHOGONAL;
		}
	}

	return ret;
}

bool Map::LoadTileSets(pugi::xml_node mapFile) {

	bool ret = true;

	pugi::xml_node tileset;
	for (tileset = mapFile.child("tileset"); tileset && ret; tileset = tileset.next_sibling("tileset"))
	{
		TileSet* set = new TileSet();
		if (ret == true) ret = LoadTilesetDetails(tileset, set);
		if (ret == true) ret = LoadTilesetImage(tileset, set);
		mapData.tilesets.add(set);
	}

	return ret;
}

bool Map::LoadTilesetDetails(pugi::xml_node& tileset_node, TileSet* set)
{
	bool ret = true;

	// L03: DONE 4: Load Tileset attributes
	set->name.Create(tileset_node.attribute("name").as_string());
	set->firstgid = tileset_node.attribute("firstgid").as_int();
	set->tileWidth = tileset_node.attribute("tilewidth").as_int();
	set->tileHeight = tileset_node.attribute("tileheight").as_int();
	set->margin = tileset_node.attribute("margin").as_int();
	set->spacing = tileset_node.attribute("spacing").as_int();
	set->tilecount = tileset_node.attribute("tilecount").as_int();
	set->columns = tileset_node.attribute("columns").as_int();

	return ret;
}

bool Map::LoadTilesetImage(pugi::xml_node& tileset_node, TileSet* set)
{
	bool ret = true;
	pugi::xml_node image = tileset_node.child("image");

	if (image == NULL)
	{
		LOG("Error parsing tileset xml file: Cannot find 'image' tag.");
		ret = false;
	}
	else
	{
		// L03: DONE 4: Load Tileset image
		SString tmp("%s%s", folder.GetString(), image.attribute("source").as_string());
		set->texture = app->tex->Load(tmp.GetString());
	}

	return ret;
}

bool Map::LoadLayer(pugi::xml_node& node, MapLayer* layer)
{
	bool ret = true;

	//Load the attributes
	layer->name = node.attribute("name").as_string();
	layer->width = node.attribute("width").as_int();
	layer->height = node.attribute("height").as_int();
	layer->offset.x = node.attribute("offsetx").as_int();
	layer->offset.y = node.attribute("offsety").as_int();

	//L06: TODO 6 Call Load Propoerties
	LoadLayerProperties(node, layer->properties);

	//Reserve the memory for the tile array
	layer->data = new uint[layer->width * layer->height];
	memset(layer->data, 0, layer->width * layer->height);

	//Iterate over all the tiles and assign the values
	pugi::xml_node tile;
	int i = 0;
	for (tile = node.child("data").child("tile"); tile && ret; tile = tile.next_sibling("tile"))
	{
		layer->data[i] = tile.attribute("gid").as_int();
		i++;
	}

	return ret;
}

bool Map::LoadAllLayers(pugi::xml_node mapNode) {
	bool ret = true;
	for (pugi::xml_node layerNode = mapNode.child("layer"); layerNode && ret; layerNode = layerNode.next_sibling("layer"))
	{
		//Load the layer
		MapLayer* mapLayer = new MapLayer();
		ret = LoadLayer(layerNode, mapLayer);

		//add the layer to the map
		mapData.layers.add(mapLayer);
	}

	return ret;
}

bool Map::LoadLayerProperties(pugi::xml_node& node, Properties& properties)
{
	bool ret = false;

	for (pugi::xml_node propertieNode = node.child("properties").child("property"); propertieNode; propertieNode = propertieNode.next_sibling("property"))
	{
		Properties::Property* p = new Properties::Property();
		p->name = propertieNode.attribute("name").as_string();
		p->value = propertieNode.attribute("value").as_int();

		properties.list.add(p);
	}

	return ret;
}

bool Map::LoadMapProperties(pugi::xml_node& node, Properties& properties)
{
	bool ret = false;

	for (pugi::xml_node propertieNode = node.child("properties").child("property"); propertieNode; propertieNode = propertieNode.next_sibling("property"))
	{
		Properties::Property* p = new Properties::Property();
		p->name = propertieNode.attribute("name").as_string();
		p->value = propertieNode.attribute("value").as_int() * mapData.tileWidth;

		properties.list.add(p);
	}

	return ret;
}

bool Map::LoadCollisions()
{
	//if (mapLoaded == false) return false;

	// L04: DONE 5: Prepare the loop to draw all tilesets + DrawTexture()
	ListItem<MapLayer*>* mapLayerItem;
	mapLayerItem = mapData.layers.start;
	
	// L06: TODO 4: Make sure we draw all the layers and not just the first one.
	while (mapLayerItem != NULL) {

		if (mapLayerItem->data->properties.GetProperty("Navigation")==1) {
			int offset = mapLayerItem->data->properties.GetProperty("sizeOffset");
			for (int x = 0; x < mapLayerItem->data->width; x++)
			{
				for (int y = 0; y < mapLayerItem->data->height; y++)
				{
					if (mapLayerItem->data->Get(x, y) == NULL)continue;
					iPoint p = MapToWorld(x, y);
					SDL_Rect rect = { (p.x + mapLayerItem->data->offset.x) - offset,(p.y + mapLayerItem->data->offset.y) - offset,mapData.tileWidth + (offset * 2),mapData.tileHeight + (offset * 2) };
					Collider* newCol = app->collisions->AddCollider(rect, Collider::Type::GROUND, app->sceneGame, NULL);
					//app->render->DrawTexture(mapData.tilesets.start->data->texture, p.x, p.y, &rect);
				}
			}
		}

		mapLayerItem = mapLayerItem->next;
	}

	SDL_Rect rect;
	rect.x = mapData.properties.GetProperty("winx");
	rect.y = mapData.properties.GetProperty("winy");
	rect.w = mapData.tileWidth;
	rect.h = mapData.tileHeight;


	app->sceneGame->winCol = app->collisions->AddCollider(rect, Collider::Type::WIN, NULL, NULL);


	iPoint dim = MapToWorld(mapData.width, mapData.height);
	rect = { 0,0,dim.x,dim.y };

	app->sceneGame->borders = app->collisions->AddCollider(rect, Collider::Type::BORDER, NULL, NULL);
	return true;
}

bool Map::LoadPlatforms()
{
	ListItem<MapLayer*>* mapLayerItem;
	mapLayerItem = mapData.layers.start;

	while (mapLayerItem != NULL) {

		if (mapLayerItem->data->properties.GetProperty("Navigation") == 2) {

			for (int x = 0; x < mapLayerItem->data->width; x++)
			{
				for (int y = 0; y < mapLayerItem->data->height; y++)
				{
					if (mapLayerItem->data->Get(x, y) == NULL)continue;
					iPoint p = MapToWorld(x - 1, y);
					app->entityManager->CreateEntity(EntityType::PLATFORM, 0, { p.x,p.y,0,0 });
				}
			}
		}


		mapLayerItem = mapLayerItem->next;
	}

	return true;
}

bool Map::LoadPositions()
{
	app->player->pos.x = mapData.properties.GetProperty("playerx");
	app->player->pos.y = mapData.properties.GetProperty("playery");


	return true;
}

bool Map::LoadEnemies()
{
	bool ret = false;
	
	ListItem<EnemyData*>* item;
	item = enemyData.start;

	while (item != NULL)
	{
		ret = true;
		
		Enemy* newEnemy = new Enemy(item->data->id, item->data->dim, item->data->detector, item->data->lim1, item->data->lim2, item->data->type);
		app->entityManager->AddEntity(newEnemy);
		item = item->next;
	}

	

	return ret;
}

bool Map::LoadCoins()
{
	bool ret = false;

	ListItem<MapLayer*>* mapLayerItem;
	mapLayerItem = mapData.layers.start;

	while (mapLayerItem != NULL) {

		if (mapLayerItem->data->properties.GetProperty("Navigation") == 5) {

			for (int x = 0; x < mapLayerItem->data->width; x++)
			{
				for (int y = 0; y < mapLayerItem->data->height; y++)
				{
					if (mapLayerItem->data->Get(x, y) == NULL)continue;
					ret = true;

					iPoint p = MapToWorld(x , y);

					Coin* coin = new Coin(0, { p.x,p.y,8,8 });
					app->entityManager->AddEntity(coin);
				}
			}
		}


		mapLayerItem = mapLayerItem->next;
	}

	return ret;
}

// L12b: Create walkability map for pathfinding
bool Map::CreateWalkabilityMap(uchar** buffer) const
{
	bool ret = false;
	ListItem<MapLayer*>* mapLayerItem;
	mapLayerItem = mapData.layers.start;

	while (mapLayerItem != NULL) {

		if (mapLayerItem->data->properties.GetProperty("Navigation", 0) == 3) {

			uchar* map = new uchar[mapLayerItem->data->width * mapLayerItem->data->height];
			memset(map, 0, mapLayerItem->data->width * mapLayerItem->data->height);

			for (int x = 0; x < mapLayerItem->data->width; x++)
			{
				for (int y = 0; y < mapLayerItem->data->height; y++)
				{
					if (mapLayerItem->data->Get(x, y) == NULL)continue;
					map[(y * mapLayerItem->data->width) + x] = 1;
				}
			}

			*buffer = map;
			ret = true;
		}

		mapLayerItem = mapLayerItem->next;
	} 


	return ret;
}

bool Map::CreateFlyabilityMap(uchar** buffer) const
{
	bool ret = false;
	ListItem<MapLayer*>* mapLayerItem;
	mapLayerItem = mapData.layers.start;

	while (mapLayerItem != NULL) {

		if (mapLayerItem->data->properties.GetProperty("Navigation",0) == 4) {
			
			uchar* map = new uchar[mapLayerItem->data->width * mapLayerItem->data->height];
			memset(map, 0, mapLayerItem->data->width * mapLayerItem->data->height);

			for (int x = 0; x < mapLayerItem->data->width; x++)
			{
				for (int y = 0; y < mapLayerItem->data->height; y++)
				{
					if (mapLayerItem->data->Get(x, y) == NULL)continue;
					map[(y * mapLayerItem->data->width) + x] = 1;
				}
			}
			
			*buffer = map;
			ret = true;
		}

		mapLayerItem = mapLayerItem->next;
	}


	return ret;
}