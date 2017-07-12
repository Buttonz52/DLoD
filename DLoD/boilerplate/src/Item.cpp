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
  if (v->getPlayerType() == human)
	v->playSFX("hitDmg", MIX_MAX_VOLUME, hitItem);
}

void empTrap(Vehicle* v)
{
	v->stun = make_pair(true, 5000 + v->timer.getTicks());
	if (v->getPlayerType() ==human)
		v->playSFX("hitStun", MIX_MAX_VOLUME, hitItem);
}

void healthPack(Vehicle* v)
{
  v->heal(10);
  if (v->getPlayerType() == human)
	v->playSFX("getHealth", MIX_MAX_VOLUME, hitItem);
}

void armourPack(Vehicle* v)
{
  v->repair(10);
  if (v->getPlayerType() == human)
	v->playSFX("armourAndSelect", MIX_MAX_VOLUME, hitItem);

}


Item::Item(ItemType type, ItemSpawner* s)
{
  setScale(vec3(3));
  spawner = s;
  
  isTrap = (spawner == nullptr);

  switch (type)
  {
  case DamageTrap:
    onPickUp = damageTrap;
    setColour(vec3(1, 1, 0));
    break;

  case EmpTrap:
    onPickUp = empTrap;
    setColour(vec3(0, 0, 1));
    break;

  case HealthPack:
    onPickUp = healthPack;
    setColour(vec3(1, 0, 0));
    break;

  case ArmourPack:
    onPickUp = armourPack;
    setColour(vec3(0, 1, 0));
    break;


  default:
    break;
  }

}


Item::~Item()
{
}

