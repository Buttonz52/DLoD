#include "Physics/PhysXMain.h"


using namespace physx;

PhysXMain::PhysXMain()
{

	PxDefaultErrorCallback gDefaultErrorCallback;
	PxDefaultAllocator gDefaultAllocatorCallback;
	PxFoundation *mFoundation = NULL;
	PxPhysics *mPhysics = NULL;
}


PhysXMain::~PhysXMain()
{
}

void PhysXMain::doVehicleStuff() {

}