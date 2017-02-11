#include "Physics/PhysXMain.h"


using namespace physx;

PhysXMain::PhysXMain()
{

}

PhysXMain::~PhysXMain()
{
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
	sceneDesc.filterShader = PxDefaultSimulationFilterShader;  //use our own shader here?? -bp
	gScene = gPhysics->createScene(sceneDesc);

	gMaterial = gPhysics->createMaterial(0.5f, 0.5f, 0.6f);	//static friction, dynamic friction, restitution

	PxRigidStatic* groundPlane = PxCreatePlane(*gPhysics, PxPlane(0, 1, 0, 0), *gMaterial);
	gScene->addActor(*groundPlane);

	//initObject();
}

void PhysXMain::initObject(GEO* g)
{
	PxShape* shape = gPhysics->createShape(PxBoxGeometry(1, 1, 2), *gMaterial);
	g->setShape(*shape);
	PxRigidDynamic *body = gPhysics->createRigidDynamic(PxTransform(PxVec3(0, 2, 0)));
	body->attachShape(*shape);
	PxRigidBodyExt::updateMassAndInertia(*body, 10.0f);
	g->setBody(*body);
	gScene->addActor(*body); //when simulate is called anything added to scene is go for sim.	
}

void PhysXMain::accelerate(GEO* g)
{
	
	PxRigidBodyExt::addForceAtLocalPos(g->getBody(), g->getBody().getGlobalPose().q.getBasisVector2()*10000, PxVec3(0, 0, 0));
	
	mat4 M = convertMat(g->getBody().getGlobalPose().q.getBasisVector0(), g->getBody().getGlobalPose().q.getBasisVector1(), g->getBody().getGlobalPose().q.getBasisVector2(), g->getBody().getGlobalPose().p);
	g->setModelMatrix(M);

	print4x4Matrix(g->getModelMatrix());


}

void PhysXMain::decelerate(GEO* g)
{

	PxRigidBodyExt::addForceAtLocalPos(g->getBody(), PxVec3(0, 0, 10000), PxVec3(0, 0, 0));

	mat4 M = convertMat(PxVec3(1, 0, 0), PxVec3(0, 1, 0), PxVec3(0, 0, 1), g->getBody().getGlobalPose().p);
	g->setModelMatrix(M);

	print4x4Matrix(g->getModelMatrix());


}

void PhysXMain::stepPhysics(bool interactive)
{
	PX_UNUSED(interactive);
	gScene->simulate(1.0f / 60.0f);
	gScene->fetchResults(true);
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