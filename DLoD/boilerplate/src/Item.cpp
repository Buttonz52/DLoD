#include "GEO\item\Item.h"


/* Create all the Item functions here
 * Need to be of the form 
 * void f(Vehicle*)
 * To create a new Item make a function which modifies the vehicle, add to the enum,
 * and add to the switch statement in the Item constructor
 */

void damageTrap(Vehicle* v)
{
  v->updateHealth(30);
}

void empTrap(Vehicle* v)
{
  v->timer.start();
  v->stun = make_pair(true, 5000);
}




Item::Item(ItemType type)
{
  switch (type)
  {
  case DamageTrap:
    onPickUp = damageTrap;
    break;

  case EmpTrap:
    onPickUp = empTrap;
    break;

  default:
    break;
  }

}


Item::~Item()
{
}

