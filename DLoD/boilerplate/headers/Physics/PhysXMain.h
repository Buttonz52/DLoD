#ifndef PHYSXMAIN_H
#define PHYSXMAIN_H

#define PVD_HOST "127.0.0.1"

#include "PxPhysicsAPI.h"
#include "PxPhysics.h"
#include "PxScene.h"
#include "vehicle\PxVehicleDrive4W.h"
#include "../GEO/vehicle/Vehicle.h"
class PhysXMain
{
public:

	static void initPhysics();
	static void initObject();
	static void stepPhysics(bool interactive);
	static void cleanupPhysics(bool interactive);
	static void linkCooking();
	static void initVehicle(Vehicle &Vehicle);
	static void setTolerancesScale(float _length, float _speed, float _mass);
	PxConvexMesh* createConvexMesh(const PxVec3* verts, const PxU32 numVerts, PxPhysics& physics, PxCooking& cooking);
	PxConvexMesh* createWheelMesh(const PxF32 width, const PxF32 radius, PxPhysics& physics, PxCooking& cooking);
	PxConvexMesh* createChassisMesh(const PxVec3 dims, PxPhysics& physics, PxCooking& cooking);
	PxRigidDynamic* createVehicleActor
	(const PxVehicleChassisData& chassisData,
		PxMaterial** wheelMaterials, PxConvexMesh** wheelConvexMeshes, const PxU32 numWheels,
		PxMaterial** chassisMaterials, PxConvexMesh** chassisConvexMeshes, const PxU32 numChassisMeshes,
		PxPhysics& physics)
};

#endif