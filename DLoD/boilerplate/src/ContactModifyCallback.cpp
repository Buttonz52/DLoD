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
  
  const PxU32 bufferSize = 64;
  PxContactPairPoint contacts[bufferSize];
  for (PxU32 i = 0; i < count; ++i)
  {
    
    const PxRigidDynamic* dynamic0 = pairs[i].actor[0]->is<PxRigidDynamic>();
    const PxRigidDynamic* dynamic1 = pairs[i].actor[1]->is<PxRigidDynamic>();

    PxU32 nbPoints = pairs[i].contacts.size();


    if (dynamic0 != NULL && dynamic1 != NULL)
    {

      Vehicle* v1 = vMap[(PxRigidBody*)dynamic0];
      Vehicle* v2 = vMap[(PxRigidBody*)dynamic1];

      // If both actors are vehicles
      if (v1 != nullptr && v2 != nullptr)
      {
		 
        for (PxU32 j = 0; j < nbPoints; ++j)
        {
          PxVec3 point = pairs[i].contacts.getPoint(j);
          PxVec3 normal = pairs[i].contacts.getNormal(j);
          normal.normalize();

          PxVec3 v1Impulse = v1->physXVehicle->getRigidDynamicActor()->getLinearVelocity();
          PxVec3 v2Impluse = v2->physXVehicle->getRigidDynamicActor()->getLinearVelocity();

          v1Impulse = (normal * (normal.dot(v1Impulse))) * v1->physXVehicle->getRigidDynamicActor()->getMass();
          v2Impluse = (normal * (normal.dot(v2Impluse))) * v2->physXVehicle->getRigidDynamicActor()->getMass();

          double forceApplied = (v1Impulse - v2Impluse).magnitude() * 0.00042;
		  forceApplied /= (double) nbPoints;

		  if (forceApplied > (10.0 / (double)nbPoints))
		  {
			  cout << "Damage: " << forceApplied << endl;
			  v1->playSFX("crash");
			  v1->calculateDamage(point.x, point.y, point.z, forceApplied);
			  v2->calculateDamage(point.x, point.y, point.z, forceApplied);
		  }

        }
      }


    }
  }
}
