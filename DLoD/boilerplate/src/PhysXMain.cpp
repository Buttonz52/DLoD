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
PxRigidStatic*					gArena = NULL;
PxVehicleDrivableSurfaceToTireFrictionPairs* gFrictionPairs = NULL;
bool					gIsVehicleInAir = true; // unused




PhysXMain::PhysXMain()
{

}

PhysXMain::~PhysXMain()
{
}

VehicleDesc PhysXMain::initMediumVehicleDesc()
{
	const PxF32 chassisMass = 1000.0;
	const PxVec3 chassisDims(4.5f, 3.0f, 10.0f);
	const PxVec3 chassisMOI
		((chassisDims.y*chassisDims.y + chassisDims.z*chassisDims.z)*chassisMass /12.0f,
			(chassisDims.x*chassisDims.x + chassisDims.z*chassisDims.z)*0.8f*chassisMass / 12.0f,
			(chassisDims.x*chassisDims.x + chassisDims.y*chassisDims.y)*chassisMass / 12.0f);
	const PxVec3 chassisCMOffset(0.0f, -chassisDims.y*0.5f + 0.65f, 0.25f);

	//Set up the wheel mass, radius, width, moment of inertia, and number of wheels.
	//Moment of inertia is just the moment of inertia of a cylinder.
	const PxF32 wheelMass = 20;
	const PxF32 wheelRadius = 0.5f;
	const PxF32 wheelWidth = 0.45f;
	const PxF32 wheelMOI = 40.f*wheelMass*wheelRadius*wheelRadius;
	const PxU32 nbWheels = 4;

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


VehicleDesc PhysXMain::initLightVehicleDesc()
{
	const PxF32 chassisMass = 1000.0;
	const PxVec3 chassisDims(4.5f, 3.0f, 10.0f);
	const PxVec3 chassisMOI
	((chassisDims.y*chassisDims.y + chassisDims.z*chassisDims.z)*chassisMass / 12.0f,
		(chassisDims.x*chassisDims.x + chassisDims.z*chassisDims.z)*0.8f*chassisMass / 12.0f,
		(chassisDims.x*chassisDims.x + chassisDims.y*chassisDims.y)*chassisMass / 12.0f);
	const PxVec3 chassisCMOffset(0.0f, -chassisDims.y*0.5f + 0.65f, 0.25f);

	//Set up the wheel mass, radius, width, moment of inertia, and number of wheels.
	//Moment of inertia is just the moment of inertia of a cylinder.
	const PxF32 wheelMass = 20;
	const PxF32 wheelRadius = 0.5f;
	const PxF32 wheelWidth = 0.15f;
	const PxF32 wheelMOI = 30.f*wheelMass*wheelRadius*wheelRadius;
	const PxU32 nbWheels = 4;

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


VehicleDesc PhysXMain::initLargeVehicleDesc()
{
	const PxF32 chassisMass = 1000.0;
	const PxVec3 chassisDims(4.5f, 3.0f, 10.0f);
	const PxVec3 chassisMOI
	((chassisDims.y*chassisDims.y + chassisDims.z*chassisDims.z)*chassisMass / 12.0f,
		(chassisDims.x*chassisDims.x + chassisDims.z*chassisDims.z)*0.8f*chassisMass / 12.0f,
		(chassisDims.x*chassisDims.x + chassisDims.y*chassisDims.y)*chassisMass / 12.0f);
	const PxVec3 chassisCMOffset(0.0f, -chassisDims.y*0.5f + 0.65f, 0.25f);

	//Set up the wheel mass, radius, width, moment of inertia, and number of wheels.
	//Moment of inertia is just the moment of inertia of a cylinder.
	const PxF32 wheelMass = 20;
	const PxF32 wheelRadius = 0.5f;
	const PxF32 wheelWidth = 0.45f;
	const PxF32 wheelMOI = 50.f*wheelMass*wheelRadius*wheelRadius;
	const PxU32 nbWheels = 4;

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

void PhysXMain::init(const int numVehicles)

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
	sceneDesc.gravity = PxVec3(0.0f, 3*-9.81f, 0.0f);
	gDispatcher = PxDefaultCpuDispatcherCreate(2);
	sceneDesc.cpuDispatcher = gDispatcher;
	sceneDesc.filterShader = VehicleFilterShader;	//this will give us heck later
	//sceneDesc.simulationEventCallback = &gContactReportCallback;

	sceneDesc.contactModifyCallback = new ContactModifyCallback(this);
	gScene = gPhysics->createScene(sceneDesc);

	gMaterial = gPhysics->createMaterial(0.5f, 0.5f, 0.0f);	//static friction, dynamic friction, restitution

	gCooking = PxCreateCooking(PX_PHYSICS_VERSION, *gFoundation, PxCookingParams(PxTolerancesScale()));

	PxInitVehicleSDK(*gPhysics);
	PxVehicleSetBasisVectors(PxVec3(0, 1, 0), PxVec3(0, 0, 1));
	PxVehicleSetUpdateMode(PxVehicleUpdateMode::eVELOCITY_CHANGE);

	//Create the batched scene queries for the suspension raycasts.
	gVehicleSceneQueryData = VehicleSceneQueryData::allocate(numVehicles, PX_MAX_NB_WHEELS, numVehicles, gAllocator);		//if we want more cars change this
	gBatchQuery = VehicleSceneQueryData::setUpBatchedSceneQuery(0, *gVehicleSceneQueryData, gScene);

	//Create the friction table for each combination of tire and surface type.
	gFrictionPairs = createFrictionPairs(gMaterial);

	//plane to drive on

	//gGroundPlane = createDrivablePlane(gMaterial, gPhysics);
	//gScene->addActor(*gGroundPlane);
}

void PhysXMain::initVehicle(Vehicle* v, int type)
{
	//Create a vehicle that will drive on the plane.
	VehicleDesc vehicleDesc;
	switch (type) {
	case 0:
		vehicleDesc = initLightVehicleDesc();
		break;
	case 1:
		vehicleDesc = initMediumVehicleDesc();
		break;
	case 2:
		vehicleDesc = initLargeVehicleDesc();
		break;
	default:
		vehicleDesc = initMediumVehicleDesc();
	}

	v->physXVehicle = createVehicleNoDrive(vehicleDesc, gPhysics, gCooking);

	//change Center of Mass
	PxTransform centerOfMass = v->physXVehicle->getRigidDynamicActor()->getCMassLocalPose();
	centerOfMass.p = PxVec3(0.0, -0.35, 0.25);		//default = (0.0, -0.35, 0.25)
	v->physXVehicle->getRigidDynamicActor()->setCMassLocalPose(centerOfMass);

	PxTransform startTransform(PxVec3(v->getPosition().x, (vehicleDesc.chassisDims.y*0.5f + vehicleDesc.wheelRadius + 1.0f)+v->getPosition().y, v->getPosition().z), PxQuat(PxIdentity));
	v->physXVehicle->getRigidDynamicActor()->setGlobalPose(startTransform);
	gScene->addActor(*v->physXVehicle->getRigidDynamicActor());
	geoMap.insert(make_pair(v->physXVehicle->getRigidDynamicActor(), v));	//lolz <---- this is great
}

/*
//attempt to get triangle mesh working for traps
void PhysXMain::initItem(Item* item)
{
PxTriangleMesh* tMesh = initTriangleMesh(item);
if (tMesh != NULL) {
PX_ASSERT(tMesh);
PxMeshScale meshScale = PxMeshScale(PxVec3(item->getScale().x, item->getScale().y, item->getScale().z), PxQuat(PxIdentity));

//fix gItem to match the PxRigidDynamic body definition
vec3 pos = item->getModelMatrix()[3];

//PxRigidDynamic *body = gPhysics->createRigidDynamic();

PxRigidDynamic *gItem = gPhysics->createRigidDynamic(PxTransform(PxVec3(pos.x, pos.y, pos.z)));
gItem->setRigidDynamicFlag(PxRigidDynamicFlag::eKINEMATIC, true);
PxShape * shape[1];
shape[0] = gItem->createShape(PxTriangleMeshGeometry(tMesh, meshScale), *gMaterial);

//PX_ASSERT(actor);
//gArena->setRigidDynamicFlag(PxRigidDynamicFlag::eKINEMATIC, true);
//PxShape * shape = gArena->createShape(PxTriangleMeshGeometry(tMesh, meshScale), *gMaterial);
//gArena->getShapes(shape, 1);

PX_ASSERT(shape);
//gScene->addActor(*actor);
shape[0]->setFlag(PxShapeFlag::eSIMULATION_SHAPE, false);
gItem->setRigidDynamicFlag(PxRigidDynamicFlag::eKINEMATIC, false);
//Set the query filter data of the ground plane so that the vehicle raycasts can hit the ground.

PxFilterData simFilterData;
simFilterData.word0 = COLLISION_FLAG_CHASSIS;
simFilterData.word1 = COLLISION_FLAG_CHASSIS_AGAINST;

PxFilterData qryFilterData;
setupNonDrivableSurface(qryFilterData);

shape[0]->setQueryFilterData(qryFilterData);
shape[0]->setSimulationFilterData(simFilterData);

gScene->addActor(*gItem);

//gItem->attachShape(*shape[0]);
geoMap.insert(make_pair(gItem, item));
}
else {
cout << "Did not init triangle mesh" << endl;
}
}
*/
void PhysXMain::initItem(Item* item)
{
  PxShape* shape = gPhysics->createShape(PxBoxGeometry(1, 1, 1), *gMaterial);

  vec3 pos = item->getModelMatrix()[3];

  PxRigidDynamic *body = gPhysics->createRigidDynamic(PxTransform(PxVec3(pos.x, pos.y, pos.z)));

  PxFilterData simFilterData;
  simFilterData.word0 = COLLISION_FLAG_CHASSIS;
  simFilterData.word1 = COLLISION_FLAG_CHASSIS_AGAINST;

  PxFilterData qryFilterData;
  setupNonDrivableSurface(qryFilterData);

  shape->setQueryFilterData(qryFilterData);
  shape->setSimulationFilterData(simFilterData);

  body->attachShape(*shape);
  gScene->addActor(*body);
  geoMap.insert(make_pair(body, item));
}



void PhysXMain::initArena(GEO *arena) {
	PxTriangleMesh* tMesh = initTriangleMesh(arena);
	if (tMesh != NULL) {
		PX_ASSERT(tMesh);
		PxMeshScale meshScale = PxMeshScale(PxVec3(arena->getScale().x, arena->getScale().y, arena->getScale().z), PxQuat(PxIdentity));
		gArena = gPhysics->createRigidStatic(PxTransform(PxIdentity));

		PX_ASSERT(actor);
		PxShape * shape[1];
		shape[0] = gArena->createShape(PxTriangleMeshGeometry(tMesh, meshScale), *gMaterial);
		gArena->getShapes(shape, 1);

		PX_ASSERT(shape);
		//Set the query filter data of the ground plane so that the vehicle raycasts can hit the ground.
		physx::PxFilterData qryFilterData;
		setupDrivableSurface(qryFilterData);
		shape[0]->setQueryFilterData(qryFilterData);

		//Set the simulation filter data of the ground plane so that it collides with the chassis of a vehicle but not the wheels.
		PxFilterData simFilterData;
		simFilterData.word0 = COLLISION_FLAG_GROUND;
		simFilterData.word1 = COLLISION_FLAG_GROUND_AGAINST;
		shape[0]->setSimulationFilterData(simFilterData);
		gScene->addActor(*gArena);
	}
	else {
		cout << "Did not init triangle mesh" << endl;
	}
}

//initialize triangle mesh
PxTriangleMesh* PhysXMain::initTriangleMesh(GEO *geo) {
	PxTriangleMeshDesc meshDesc;

	vector<PxVec3> verts;
	vector<PxU32> indices;
	for (int i = 0; i < geo->mesh.vertices.size(); i++) {
		vec3 p = geo->mesh.vertices.at(i);
		verts.push_back(PxVec3(p.x, p.y, p.z));
	}
	for (int i = 0; i < geo->mesh.faces.size(); i++) {
		indices.push_back(geo->mesh.faces.at(i));
	}

	meshDesc.points.count = verts.size();
	meshDesc.points.stride = sizeof(PxVec3);
	meshDesc.points.data = &verts[0];

	meshDesc.triangles.count = indices.size()/3;
	meshDesc.triangles.stride = 3 * sizeof(PxU32);
	meshDesc.triangles.data = &indices[0];

	PxDefaultMemoryOutputStream writeBuffer;
	bool status = gCooking->cookTriangleMesh(meshDesc, writeBuffer);
	if (!status)
		return NULL;
	PxDefaultMemoryInputData readBuffer(writeBuffer.getData(), writeBuffer.getSize());
	return gPhysics->createTriangleMesh(readBuffer);
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

  map<PxRigidActor*, GEO*>::iterator itr = geoMap.begin();
  PxRigidActor* body;

  while (itr != geoMap.end()) 
  {
    PxRigidActor* body = itr->first;
    if (body != nullptr && itr->second != nullptr)
    {
      mat4 M = convertMat(body->getGlobalPose().q.getBasisVector0(), body->getGlobalPose().q.getBasisVector1(), body->getGlobalPose().q.getBasisVector2(), body->getGlobalPose().p);
      itr->second->setModelMatrix(M);
    }

    ++itr;
  }

	for (Vehicle* v : vehiclesVec)
	{
		v->updateWheelPosition();
		v->updateArmour();
		v->releaseAllControls();
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

