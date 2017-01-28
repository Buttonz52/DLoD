#ifndef ITEM_H
#define ITEM_H
#include "../GEO.H"
class Item :
	public GEO
{
public:
	Item();
	~Item();

	virtual int ReturnValue();
};

#endif