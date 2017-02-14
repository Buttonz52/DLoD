#ifndef CONTACTMODIFYCALLBACK_H
#define CONTACTMODIFYCALLBACK_H
/*
  ContactModifyCallback class
  
  
  */
#include "PxPhysicsAPI.h"
#include "PxPhysics.h"
#include "PxScene.h"
#include "Physics/PhysXMain.h"


class PhysXMain;

class ContactModifyCallback : public PxContactModifyCallback
{
private:
  PhysXMain* physX = nullptr;


public:
  
  ContactModifyCallback();
  ContactModifyCallback(PhysXMain*);

  void onContactModify(PxContactModifyPair* const pairs, PxU32 count);

};



#endif