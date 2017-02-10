#ifndef PHYSXMAIN_H
#define PHYSXMAIN_H

#define PVD_HOST "127.0.0.1"

#include "PxPhysicsAPI.h"
#include "PxPhysics.h"
#include "PxScene.h"


class PhysXMain
{
public:
	PhysXMain();
	~PhysXMain();
	void doVehicleStuff();
};

#endif