#ifndef PHYSXMAIN_H
#define PHYSXMAIN_H

#define PVD_HOST "127.0.0.1"

#include "PxPhysicsAPI.h"
#include "PxPhysics.h"
#include "PxScene.h"
#include "../GEO/GEO.h"

using namespace physx;

class PhysXMain
{
public:

	PxDefaultAllocator		gAllocator;
	PxDefaultErrorCallback	gErrorCallback;
	PxFoundation*			gFoundation = NULL;
	PxPhysics*				gPhysics = NULL;
	PxDefaultCpuDispatcher*	gDispatcher = NULL;
	PxScene*				gScene = NULL;
	PxCooking*				gCooking = NULL;
	PxMaterial*				gMaterial = NULL;
	PxVisualDebuggerConnection* gConnection = NULL;
	//VehicleSceneQueryData*	gVehicleSceneQueryData = NULL;	//for when we actually do car stuff -bp
	PxBatchQuery*			gBatchQuery = NULL;
	PxVehicleDrivableSurfaceToTireFrictionPairs* gFrictionPairs = NULL;
	PxRigidStatic*			gGroundPlane = NULL;
	PxVehicleDrive4W*		gVehicle4W = NULL;
	bool					gIsVehicleInAir = true; // unused

	PhysXMain();
	~PhysXMain();
	void init();
	void initObject(GEO* g);

	void accelerate(GEO* g);
	void decelerate(GEO* g);
	void turn(GEO* g, float dir);

	void stepPhysics(bool interactive, GEO* g);
	void cleanupPhysics(bool interactive);

	mat4 convertMat(PxVec3 x, PxVec3 y, PxVec3 z, PxVec3 w);
};

#endif