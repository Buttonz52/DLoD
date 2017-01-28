#ifndef ARMOURPACK_H
#define ARMOURPACK_H
#include "Item.h"
class ArmourPack :
	public Item
{
public:
	ArmourPack();
	~ArmourPack();

	int ReturnValue();

private: 
	int ap;
};
#endif
