#ifndef PHYSXMAIN_H
#define PHYSXMAIN_H

#define PVD_HOST "127.0.0.1"

#include "PxPhysicsAPI.h"
#include "PxPhysics.h"
#include "PxScene.h"
#include "../GEO/GEO.h"
#include "../GEO/vehicle/Vehicle.h"
#include "Physics\SnippetVehicleRaycast.h"
#include "Physics\SnippetVehicleCreate.h"
#include "Physics\SnippetVehicleTireFriction.h"
#include "Physics\SnippetVehicleFilterShader.h"

using namespace physx;


class PhysXMain
{
public:
	PhysXMain();
	~PhysXMain();
	void init();
	VehicleDesc initVehicleDesc();
	void initVehicle(Vehicle* v);
	void initObject(GEO* g);

	void accelerate(Vehicle* g, float m);
	void decelerate(Vehicle* g, float m);
	void turn(Vehicle* v, float dir);
	void brake(Vehicle* v, float brake);
	void releaseAllControls(Vehicle* v);

	void stepPhysics(bool interactive, vector<GEO *> g);
	void cleanupPhysics(bool interactive);

	mat4 convertMat(PxVec3 x, PxVec3 y, PxVec3 z, PxVec3 w);
};

#endif