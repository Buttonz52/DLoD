#ifndef PHYSXMAIN_H
#define PHYSXMAIN_H

#define PVD_HOST "127.0.0.1"

#include "PxPhysicsAPI.h"
#include "PxPhysics.h"
#include "PxScene.h"
#include "GEO/item/Item.h"
#include "GEO/vehicle/Vehicle.h"
#include "Physics\SnippetVehicleRaycast.h"
#include "Physics\SnippetVehicleCreate.h"
#include "Physics\SnippetVehicleTireFriction.h"
#include "Physics\SnippetVehicleFilterShader.h"
#include "Physics\ContactModifyCallback.h"
#include <map>

using namespace physx;

struct ContactModFlags
{
	enum
	{
		eADJUST_MASS_RATIOS = 1,
		eIGNORE_CONTACT = 2,
		eMODIFY_NORMAL = 4,
		eADJUST_PENETRATION = 8,
		eMAX_IMPULSE = 16,
		eTARGET_VELOCITY = 32
	};
};

class PhysXMain
{
private:
  map<PxRigidActor*, GEO*> geoMap;

  PxDefaultAllocator		      gAllocator;
  PxDefaultErrorCallback    	gErrorCallback;
  PxFoundation*			          gFoundation = NULL;
  PxPhysics*				          gPhysics = NULL;
  PxDefaultCpuDispatcher*   	gDispatcher = NULL;
  PxScene*				            gScene = NULL;
  PxCooking*				          gCooking = NULL;
  PxMaterial*				          gMaterial = NULL;
  PxVisualDebuggerConnection* gConnection = NULL;
  VehicleSceneQueryData*	    gVehicleSceneQueryData = NULL;
  PxBatchQuery*			          gBatchQuery = NULL;
  PxRigidStatic*			        gGroundPlane = NULL;
  PxVehicleDrivableSurfaceToTireFrictionPairs* gFrictionPairs = NULL;
  bool					gIsVehicleInAir = true; // unused


public:
	PhysXMain();
	~PhysXMain();

  vector<GEO*> deletedGeos;

	void init(const int numVehicles);
	VehicleDesc initLightVehicleDesc();
	VehicleDesc initMediumVehicleDesc();
	VehicleDesc initLargeVehicleDesc();
	void initVehicle(Vehicle* v, int type);	// 0 = light, 1 = medium, 2 = large

	void initItem(Item * item);

	void initArena(GEO *arena);
	PxTriangleMesh *initTriangleMesh(GEO * geo);

	void collisionFunction(PxContactModifyPair* const, PxU32);

	void stepPhysics(bool interactive, vector<GEO *> g);
	void cleanupPhysics(bool interactive);

	mat4 convertMat(PxVec3 x, PxVec3 y, PxVec3 z, PxVec3 w);
};

#endif