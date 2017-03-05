#ifndef ITEM_H
#define ITEM_H

#include "GEO\GEO.h"
#include "GEO\vehicle\Vehicle.h"

enum ItemType
{
  DamageTrap,

};


class Item :
	public GEO
{

public:
	Item(ItemType);
	~Item();


  bool isActive;

  void (*onPickUp)(Vehicle* v);

};

#endif