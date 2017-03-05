#ifndef DAMAGETRAP_H
#define DAMAGETRAP_H

#include "Item.h"

class DamageTrap
  : public Item
{


public:

  
  void onPickUp(Vehicle* v);

};

#endif
