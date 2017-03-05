#ifndef ITEM_H
#define ITEM_H
#include "../GEO.H"
#include "../vehicle/Vehicle.h"
class Item :
	public GEO
{
public:
	Item();
	~Item();

  bool isActive;

  //void onPickUp(Vehicle* v);

};

#endif