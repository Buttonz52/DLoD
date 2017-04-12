#ifndef ITEM_SPAWNER_H
#define ITEM_SPAWNER_H

#include "GEO\item\Item.h"
#include "Game\Timer.h"

class Item;

vector<string> itemModels =
{
  "/ObjModels/bearTrap.obj",
  "/ObjModels/bearTrap.obj"
};

class ItemSpawner
{
public:
  vec3 position;
  vector<ItemType> itemTypes;
  Item* item;

  Timer timer;

  ItemSpawner(vector<ItemType>, vec3);
  ~ItemSpawner();

  void spawnItem(GEO*);
  void initItem(Item*, GEO*);
};

#endif 
