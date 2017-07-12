#ifndef ITEM_H
#define ITEM_H

#include "GEO\GEO.h"
#include "GEO\vehicle\Vehicle.h"
#include "GEO\item\ItemSpawner.h"

enum ItemType
{
  DamageTrap = 0,
  EmpTrap = 1,
  HealthPack = 2,
  ArmourPack = 3,
};

class ItemSpawner;

class Item :
	public GEO
{
public:
	//Item(ItemType, ItemSpawner*);
	Item(ItemType type, ItemSpawner * s);
	~Item();

  bool isTrap;

  ItemSpawner* spawner;

  void (*onPickUp)(Vehicle* v);	//function as variables
};

#endif