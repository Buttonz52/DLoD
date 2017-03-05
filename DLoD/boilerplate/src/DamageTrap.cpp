#include "GEO\item\DamageTrap.h"


void DamageTrap::onPickUp(Vehicle* v)
{
  v->updateHealth(30);
}
