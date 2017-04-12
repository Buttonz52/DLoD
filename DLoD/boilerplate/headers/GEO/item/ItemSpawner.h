#ifndef ITEM_SPAWNER_H
#define ITEM_SPAWNER_H

#include "GEO\item\Item.h"
#include "Game\Timer.h"

class Item;

//vector<string> itempickModels =
//{
//  "/ObjModels/bearTrap.obj",
//  "/ObjModels/bearTrap.obj"
//};

class ItemSpawner
{
public:
  vec3 position;
  vector<int> itemTypes;
  Item* item;

  double spawnTime; // 20 seconds

  Timer timer;

  ItemSpawner(vector<int>, vec3);
  ~ItemSpawner();

  void spawnItem(GEO*);
};

#endif 
