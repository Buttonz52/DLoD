#include "Physics/ContactModifyCallback.h"

ContactModifyCallback::ContactModifyCallback()
{
  physX = nullptr;
}

ContactModifyCallback::ContactModifyCallback(PhysXMain* phx)
{
  physX = phx;
}

void ContactModifyCallback::onContactModify(PxContactModifyPair* const pairs, PxU32 count)
{
  physX->collisionFunction(pairs, count);
}


void PhysXMain::collisionFunction(PxContactModifyPair* const pairs, PxU32 count)
{
  
  for (PxU32 i = 0; i < count; i++)
  {
    
    const PxRigidDynamic* dynamic0 = pairs[i].actor[0]->is<PxRigidDynamic>();
    const PxRigidDynamic* dynamic1 = pairs[i].actor[1]->is<PxRigidDynamic>();

    if (dynamic0 != NULL && dynamic1 != NULL)
    {
      Vehicle* v1 = vMap[(PxRigidBody*)dynamic0];
      if (v1 != nullptr)
      {
        v1->calculateDamage();
      }
      else
        cout << "Could not calculate damage!" << endl;

      Vehicle* v2 = vMap[(PxRigidBody*)dynamic0];
      if (v2 != nullptr)
      {
        v2->calculateDamage();
		v2->playSFX("crash");
      }
      else
        cout << "Could not calculate damage!" << endl;

    }
  }
}
