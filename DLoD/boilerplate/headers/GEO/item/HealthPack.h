#ifndef HEALTHPACK_H
#define HEALTHPACK_H
#include "Item.h"
class HealthPack :
	public Item
{
public:
	HealthPack();
	~HealthPack();

	int ReturnValue();
private:
	int hp;
};

#endif