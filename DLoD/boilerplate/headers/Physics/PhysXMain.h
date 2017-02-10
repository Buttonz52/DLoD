#ifndef PHYSXMAIN_H
#define PHYSXMAIN_H

#define PVD_HOST "127.0.0.1"

#include "PxPhysicsAPI.h"
#include "PxPhysics.h"
#include "PxScene.h"


class PhysXMain
{
public:

	static void initPhysics();
	static void initObject();
	static void stepPhysics(bool interactive);
	static void cleanupPhysics(bool interactive);


};

#endif