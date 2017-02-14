#include "Physics/PhysXMain.h"


using namespace physx;


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


PhysXMain::PhysXMain()
{

}

PhysXMain::~PhysXMain()
{
}

VehicleDesc PhysXMain::initVehicleDesc()
{
	const PxF32 chassisMass = 1500.0;
	const PxVec3 chassisDims(5.0f, 2.0f, 5.0f);
	const PxVec3 chassisMOI
		((chassisDims.y*chassisDims.y + chassisDims.z*chassisDims.z)*chassisMass / 12.0f,
			(chassisDims.x*chassisDims.x + chassisDims.z*chassisDims.z)*0.8f*chassisMass / 12.0f,
			(chassisDims.x*chassisDims.x + chassisDims.y*chassisDims.y)*chassisMass / 12.0f);
	const PxVec3 chassisCMOffset(0.0f, -chassisDims.y*0.5f + 0.65f, 0.25f);

	//Set up the wheel mass, radius, width, moment of inertia, and number of wheels.
	//Moment of inertia is just the moment of inertia of a cylinder.
	const PxF32 wheelMass = 20.0f;
	const PxF32 wheelRadius = 0.5f;
	const PxF32 wheelWidth = 0.4f;
	const PxF32 wheelMOI = 0.5f*wheelMass*wheelRadius*wheelRadius;
	const PxU32 nbWheels = 6;

	VehicleDesc vehicleDesc;
	vehicleDesc.chassisMass = chassisMass;
	vehicleDesc.chassisDims = chassisDims;
	vehicleDesc.chassisMOI = chassisMOI;
	vehicleDesc.chassisCMOffset = chassisCMOffset;
	vehicleDesc.chassisMaterial = gMaterial;
	vehicleDesc.wheelMass = wheelMass;
	vehicleDesc.wheelRadius = wheelRadius;
	vehicleDesc.wheelWidth = wheelWidth;
	vehicleDesc.wheelMOI = wheelMOI;
	vehicleDesc.numWheels = nbWheels;
	vehicleDesc.wheelMaterial = gMaterial;
	return vehicleDesc;
}

void PhysXMain::init()
{
	gFoundation = PxCreateFoundation(PX_PHYSICS_VERSION, gAllocator, gErrorCallback);
	PxProfileZoneManager* profileZoneManager = &PxProfileZoneManager::createProfileZoneManager(gFoundation);
	gPhysics = PxCreatePhysics(PX_PHYSICS_VERSION, *gFoundation, PxTolerancesScale(), true, profileZoneManager);
	
	if (gPhysics->getPvdConnectionManager()) //will likely not need?
	{
		gPhysics->getVisualDebugger()->setVisualizeConstraints(true);
		gPhysics->getVisualDebugger()->setVisualDebuggerFlag(PxVisualDebuggerFlag::eTRANSMIT_CONTACTS, true);
		gPhysics->getVisualDebugger()->setVisualDebuggerFlag(PxVisualDebuggerFlag::eTRANSMIT_SCENEQUERIES, true);
		gConnection = PxVisualDebuggerExt::createConnection(gPhysics->getPvdConnectionManager(), PVD_HOST, 5425, 10);
	}

	PxSceneDesc sceneDesc(gPhysics->getTolerancesScale());
	sceneDesc.gravity = PxVec3(0.0f, -9.81f, 0.0f);
	gDispatcher = PxDefaultCpuDispatcherCreate(2);
	sceneDesc.cpuDispatcher = gDispatcher;
	sceneDesc.filterShader = VehicleFilterShader;	//this will give us heck later
	//sceneDesc.simulationEventCallback = &gContactReportCallback;

	sceneDesc.contactModifyCallback = new ContactModifyCallback(this);
	gScene = gPhysics->createScene(sceneDesc);

	gMaterial = gPhysics->createMaterial(0.5f, 0.5f, 0.2f);	//static friction, dynamic friction, restitution

	gCooking = PxCreateCooking(PX_PHYSICS_VERSION, *gFoundation, PxCookingParams(PxTolerancesScale()));

	PxInitVehicleSDK(*gPhysics);
	PxVehicleSetBasisVectors(PxVec3(0, 1, 0), PxVec3(0, 0, 1));
	PxVehicleSetUpdateMode(PxVehicleUpdateMode::eVELOCITY_CHANGE);

	//Create the batched scene queries for the suspension raycasts.
	gVehicleSceneQueryData = VehicleSceneQueryData::allocate(1, PX_MAX_NB_WHEELS, 1, gAllocator);		//if we want more cars change this
	gBatchQuery = VehicleSceneQueryData::setUpBatchedSceneQuery(0, *gVehicleSceneQueryData, gScene);

	//Create the friction table for each combination of tire and surface type.
	gFrictionPairs = createFrictionPairs(gMaterial);

	//plane to drive on
	gGroundPlane = createDrivablePlane(gMaterial, gPhysics);
	gScene->addActor(*gGroundPlane);
}

void PhysXMain::initVehicle(Vehicle* v)
{
	//Create a vehicle that will drive on the plane.
	VehicleDesc vehicleDesc = initVehicleDesc();
	v->physXVehicle = createVehicleNoDrive(vehicleDesc, gPhysics, gCooking);
	PxTransform startTransform(PxVec3(v->getPosition().x, (vehicleDesc.chassisDims.y*0.5f + vehicleDesc.wheelRadius + 1.0f), v->getPosition().z), PxQuat(PxIdentity));
	v->physXVehicle->getRigidDynamicActor()->setGlobalPose(startTransform);
	gScene->addActor(*v->physXVehicle->getRigidDynamicActor());
	vMap.insert(make_pair(v->physXVehicle->getRigidDynamicActor(), v));	//lolz <---- this is great
}


void PhysXMain::initObject(GEO* g)
{
	PxShape* shape = gPhysics->createShape(PxBoxGeometry(2, 1, 2), *gMaterial);
	g->setShape(*shape);
	PxRigidDynamic *body = gPhysics->createRigidDynamic(PxTransform(PxVec3(0, 2, 0)));
	body->setAngularDamping(13);
	body->attachShape(*shape);
	PxRigidBodyExt::updateMassAndInertia(*body, 20.0f);
	g->setBody(*body);
	gScene->addActor(*body); //when simulate is called anything added to scene is go for sim.	
}


void PhysXMain::stepPhysics(bool interactive, vector<GEO *> g)
{
	vector<Vehicle*> vehiclesVec;
	vector<GEO*> geosVec;

	for (int i = 0; i < g.size(); i++)
	{
		if (Vehicle* v = static_cast<Vehicle*>(g[i])) {
			vehiclesVec.push_back(v);
		}
		else {
			geosVec.push_back(g[i]);
		}
	}

	vector<PxVehicleWheels*> vehicles;
	PxWheelQueryResult wheelQueryResults[PX_MAX_NB_WHEELS];
	vector<PxVehicleWheelQueryResult> vehicleQueryResults;
	for (int i = 0; i < vehiclesVec.size(); ++i) {
		vehicles.push_back(vehiclesVec[i]->physXVehicle);
		vehicleQueryResults.push_back({ wheelQueryResults, vehiclesVec[i]->physXVehicle->mWheelsSimData.getNbWheels() });
	}

	const PxF32 timestep = 1.0f / 60.0f;
	//Raycasts.
	PxRaycastQueryResult* raycastResults = gVehicleSceneQueryData->getRaycastQueryResultBuffer(0);
	const PxU32 raycastResultsSize = gVehicleSceneQueryData->getRaycastQueryResultBufferSize();
	PxVehicleSuspensionRaycasts(gBatchQuery, vehicles.size(), &vehicles[0], raycastResultsSize, raycastResults);

	//Vehicle update.
	const PxVec3 grav = gScene->getGravity();
	PxVehicleUpdates(timestep, grav, *gFrictionPairs, vehicles.size(), &vehicles[0], &vehicleQueryResults[0]);

	//Scene update.
	gScene->simulate(timestep);
	gScene->fetchResults(true);

	//will want to bring in a vector<GEO* > GEOs to go through their models and update them here
	for (Vehicle* v : vehiclesVec)
	{
		PxRigidDynamic* body = v->physXVehicle->getRigidDynamicActor();
		mat4 M = convertMat(body->getGlobalPose().q.getBasisVector0(), body->getGlobalPose().q.getBasisVector1(), body->getGlobalPose().q.getBasisVector2(), body->getGlobalPose().p);
		v->setModelMatrix(M);
		v->releaseAllControls();
	}

	for (GEO* g : geosVec)
	{
		PxRigidDynamic* body = g->getBody();
		mat4 M = convertMat(body->getGlobalPose().q.getBasisVector0(), body->getGlobalPose().q.getBasisVector1(), body->getGlobalPose().q.getBasisVector2(), body->getGlobalPose().p);
		g->setModelMatrix(M);
	}

}

void PhysXMain::cleanupPhysics(bool interactive)
{
	PX_UNUSED(interactive);
	gScene->release();
	gDispatcher->release();
	PxProfileZoneManager* profileZoneManager = gPhysics->getProfileZoneManager();
	if (gConnection != NULL)
		gConnection->release();
	gPhysics->release();
	profileZoneManager->release();
	gFoundation->release();

	printf("PhysX done.\n");
}

mat4 PhysXMain::convertMat(PxVec3 x, PxVec3 y, PxVec3 z, PxVec3 w)
{
	mat4 M = mat4(x.x, y.x, z.x, w.x,
				x.y, y.y, z.y, w.y,
				x.z, y.z, z.z, w.z,
				0.0, 0.0, 0.0, 1.0f);

	M = glm::transpose(M);

	return M;
}

