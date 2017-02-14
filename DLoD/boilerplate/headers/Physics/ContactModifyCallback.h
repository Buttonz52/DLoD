#ifndef CONTACTMODIFYCALLBACK_H
#define CONTACTMODIFYCALLBACK_H
/*
  ContactModifyCallback class
  
  
  */
#include "PxPhysicsAPI.h"
#include "PxPhysics.h"
#include "PxScene.h"


class PhysXMain;

class ContactModifyCallback : public PxContactModifyCallback
{
private:
  PhysXMain* physx;

public:
  
  ContactModifyCallback();
  ContactModifyCallback(PhysXMain*);

  void onContactModify(PxContactModifyPair* const pairs, PxU32 count);

};



#endif