#ifndef TRAPPACK_H
#define TRAPPACK_H
#include "Item.h"
class TrapPack :
	public Item
{
public:
	TrapPack();
	~TrapPack();

	int ReturnValue();

private:
	int ammo;

};

#endif