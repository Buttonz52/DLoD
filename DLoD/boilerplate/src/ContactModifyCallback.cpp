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

      Vehicle* v1 = dynamic_cast<Vehicle*>(geoMap[(PxRigidBody*)dynamic0]);
      Vehicle* v2 = dynamic_cast<Vehicle*>(geoMap[(PxRigidBody*)dynamic1]);

      Item* i1 = dynamic_cast<Item*>(geoMap[(PxRigidBody*)dynamic0]);
      Item* i2 = dynamic_cast<Item*>(geoMap[(PxRigidBody*)dynamic1]);


      // If both actors are vehicles
      if (v1 != nullptr && v2 != nullptr)
      {
        v1->recentlyHit = make_pair(true, 2000 + v1->timer.getTicks());
        v2->recentlyHit = make_pair(true, 2000 + v1->timer.getTicks());

        for (PxU32 j = 0; j < nbPoints; ++j)
        {
          PxVec3 point = pairs[i].contacts.getPoint(j);
          PxVec3 normal = pairs[i].contacts.getNormal(j);
          normal.normalize();

          PxVec3 v1Impulse = v1->physXVehicle->getRigidDynamicActor()->getLinearVelocity();
          PxVec3 v2Impluse = v2->physXVehicle->getRigidDynamicActor()->getLinearVelocity();

          v1Impulse = (normal * (normal.dot(v1Impulse))) * v1->physXVehicle->getRigidDynamicActor()->getMass();
          v2Impluse = (normal * (normal.dot(v2Impluse))) * v2->physXVehicle->getRigidDynamicActor()->getMass();

          double forceApplied = (v1Impulse - v2Impluse).magnitude() * 0.00042;				//magic number for damage
		      forceApplied /= (double) nbPoints;

          if (v1->isDead() || v2->isDead()) 
            forceApplied *= 0.25;

		      if (forceApplied > (10.0 / (double)nbPoints))
		      {
			      v1->calculateDamage(point.x, point.y, point.z, forceApplied);
			      v2->calculateDamage(point.x, point.y, point.z, forceApplied);

            v1->playSFX("crash", MIX_MAX_VOLUME, select);
				   // if (v1->isDead() || v2->isDead()) 
					//    v1->playSFX("explosion", MIX_MAX_VOLUME, 2);
		      }
        }
      }


      // If one actor is a vehicle and one is an item
      if ((v1 != nullptr && i2 != nullptr) || (v2 != nullptr && i1 != nullptr))
      {

        Vehicle* car = (v1 != nullptr) ? v1 : v2;
        Item* item = (i1 != nullptr) ? i1 : i2;

        for (PxU32 j = 0; j < nbPoints; ++j)
        {
          pairs[i].contacts.setMaxImpulse(j, 0);
        }

        item->onPickUp(car);

        if (item->spawner != nullptr) {
          item->spawner->item = nullptr;
          item->spawner->timer.start();
        }

        // remove the item
        map<PxRigidActor*, GEO*>::iterator itr = geoMap.begin();
        while (itr != geoMap.end()) {
          if (itr->second == item) {
            gScene->removeActor(*itr->first);
            deletedGeos.push_back(itr->second);
            itr = geoMap.erase(itr);
            break;
          }
          else {
            ++itr;
          }
        }
      }


      // If both are items
      if (i1 != nullptr && i2 != nullptr)
      {
        for (PxU32 j = 0; j < nbPoints; ++j)
        {
          pairs[i].contacts.setMaxImpulse(j, 0);
        }

        // remove the item
        /*
        map<PxRigidActor*, GEO*>::iterator itr = geoMap.begin();
        while (itr != geoMap.end()) {
          if (itr->second == i2) {
            gScene->removeActor(*itr->first);
            itr = geoMap.erase(itr);
            break;
          }
          else {
            ++itr;
          }
        } */
      }

    }
  }
}
