#include "stdafx.h"
#include "Physx.h"

void Physx::initPhysx()
{
	//-----------------------------CREATE FOUNDATION--------------------------------
	gFoundation = PxCreateFoundation(PX_PHYSICS_VERSION, gDefaultAllocatorCallback,
		gDefaultErrorCallback);
	if (!gFoundation)
		std::cout << "PxCreateFoundation failed!" << std::endl;

	//--------------------------------CREATE VISUAL DEBUGGER CONNECTION-----------------------
	gPvd = PxCreatePvd(*gFoundation);
	PxPvdTransport* transport = PxDefaultPvdSocketTransportCreate("127.0.0.1", 5425, 10);
	gPvd->connect(*transport, PxPvdInstrumentationFlag::eALL);

	//--------------------------------CREATE SDK---------------------------------------
	bool recordMemoryAllocations = true;
	PxTolerancesScale scale;
	scale.length = 100; //example target 1m, actual units 1cm
	scale.speed = 981; //gravity*1s

	gPhysics = PxCreatePhysics(PX_PHYSICS_VERSION, *gFoundation,
		PxTolerancesScale(scale), recordMemoryAllocations, gPvd);
	if (!gPhysics)
		std::cout << "PxCreatePhysics failed!" << std::endl;

	//--------------------------------CREATE Cooking---------------------------------------


	gCooking = PxCreateCooking(PX_PHYSICS_VERSION, *gFoundation, PxCookingParams(scale));
	if (!gCooking)
		std::cout << "PxCreateCooking failed!" << std::endl;

	//--------------------------------CREATE SCENE---------------------------------------
	PxSceneDesc sceneDesc(gPhysics->getTolerancesScale());
	sceneDesc.gravity = PxVec3(0.0f, -9.8f, 0.0f);
	sceneDesc.cpuDispatcher = PxDefaultCpuDispatcherCreate(1);
	sceneDesc.filterShader = PxDefaultSimulationFilterShader;
	gScene = gPhysics->createScene(sceneDesc);

	//--------------------------------CREATE PVD Client---------------------------------------
	PxPvdSceneClient *pvdClient = gScene->getScenePvdClient();
	if (pvdClient)
	{
		pvdClient->setScenePvdFlag(PxPvdSceneFlag::eTRANSMIT_CONSTRAINTS, true);
		pvdClient->setScenePvdFlag(PxPvdSceneFlag::eTRANSMIT_CONTACTS, true);
		pvdClient->setScenePvdFlag(PxPvdSceneFlag::eTRANSMIT_SCENEQUERIES, true);
	}

	//----------------------------------------------------------------------------------------
	//----------------------------------------------------------------------------------------
	//-----------------------------TEST SCENE-------------------------------------------------
	//----------------------------------------------------------------------------------------
	//----------------------------------------------------------------------------------------

	/*PxMaterial *mMaterial = gPhysics->createMaterial(0.5, 0.5, 0.5);
	//PxTransform planePos = PxTransform(PxVec3(0.0f, 0, 0.0f), PxQuat(PxHalfPi, PxVec3(0.0f, 0.0f, 1.0f)));
	//PxRigidStatic *plane = gPhysics->createRigidStatic(planePos);
	//PxShape *shape = gPhysics->createShape(PxPlaneGeometry(), *mMaterial);
	//plane->attachShape(*shape);
	//gScene->addActor(*plane);
	PxRigidDynamic *gBox;
	PxTransform boxPos(PxVec3(0.0f, 10.0f, 0.0f));
	PxBoxGeometry boxGeometry(PxVec3(0.5f, 0.5f, 0.5f));
	gBox = PxCreateDynamic(*gPhysics, boxPos, boxGeometry, *mMaterial, 1.0f);
	gScene->addActor(*gBox);
	PxRigidStatic *floorBox;
	PxTransform floorBoxPos(PxVec3(0.055158f, 0.0f, 0.154836f));
	PxBoxGeometry floorBoxGeometry(PxVec3(3.0221f, 0.369366f, 2.80128f));
	PxShape *floorShape = gPhysics->createShape(floorBoxGeometry, *mMaterial);
	floorBox = gPhysics->createRigidStatic(floorBoxPos);
	floorBox->attachShape(*floorShape);
	gScene->addActor(*floorBox);
	PxRigidStatic *floorUpBox;
	PxTransform floorUpBoxPos(PxVec3(-4.08367f, 0.974742f, 0.154836f), PxQuat(-0.7176811337, PxVec3(0.0f, 0.0f, 1.0f)));
	PxBoxGeometry floorUpBoxGeometry(PxVec3(3.0221f, 0.369366f, 2.80128f));
	PxShape *floorUpShape = gPhysics->createShape(floorUpBoxGeometry, *mMaterial);
	floorUpBox = gPhysics->createRigidStatic(floorUpBoxPos);
	floorUpBox->attachShape(*floorUpShape);
	gScene->addActor(*floorUpBox);
	PxRigidStatic *floorDownBox;
	PxTransform floorDownBoxPos(PxVec3(5.00811f, -2.01568f, 0.154836f), PxQuat(-0.7176811337, PxVec3(0.0f, 0.0f, 1.0f)));
	PxBoxGeometry floorDownBoxGeometry(PxVec3(3.0221f, 0.369366f, 2.80128f));
	PxShape *floorDownShape = gPhysics->createShape(floorDownBoxGeometry, *mMaterial);
	floorDownBox = gPhysics->createRigidStatic(floorDownBoxPos);
	floorDownBox->attachShape(*floorDownShape);
	gScene->addActor(*floorDownBox);
	PxRigidStatic *wallBox;
	PxTransform wallBoxPos(PxVec3(-0.083447f, 1.98391f, -2.85165f));
	PxBoxGeometry wallBoxGeometry(PxVec3(6.13943f, 5.04434f, 0.447101f));
	PxShape *wallShape = gPhysics->createShape(wallBoxGeometry, *mMaterial);
	wallBox = gPhysics->createRigidStatic(wallBoxPos);
	wallBox->attachShape(*wallShape);
	gScene->addActor(*wallBox);*/


	//----------------------------------------------------------------------------------------
	//-----------------------------CHARACTER CONTROLLER---------------------------------------
	//----------------------------------------------------------------------------------------

	manager = PxCreateControllerManager(*gScene);
	PxCapsuleControllerDesc desc;
	manager->setOverlapRecoveryModule(true);
	desc.position = PxExtendedVec3(-9.26916f, 14.4f, -15.9479f); //initial position
	desc.contactOffset = 0.1f; //controller skin within which contacts generated
	desc.stepOffset = 0.01; //max obstacle height the character can climb
	desc.slopeLimit = cosf(glm::radians(45.0f)); // max slope the character can walk
	desc.radius = 0.3f; //radius of the capsule
	desc.height = 0.5f; //height of the controller
	desc.upDirection = PxVec3(0, 1, 0); // Specifies the 'up' direction
	desc.material = gPhysics->createMaterial(0.5f, 0.5f, 0.1f); // material defines physical properties like friction, bounciness etc. - see PhysX materials
	controller = manager->createController(desc);

	PxRigidDynamic *cActor = controller->getActor();
	PxShape *cShapes[1];
	cActor->getShapes(cShapes, 1, 0);
	PxShape *cShape = cShapes[0];
	cShape->setLocalPose(PxTransform(PxQuat(1.5708, PxVec3(0, 0, 1))));
}

void Physx::createModels(std::vector<Model> models) {
	for (Model md : models) {
		std::vector<PxVec3> vertices;

		for (Mesh m : md.meshes) {
			for (Vertex v : m.vertices) {
				bool contains = false;
				PxVec3 key;
				key[0] = v.position[0];
				key[1] = v.position[1];
				key[2] = v.position[2];
				for (PxVec3 check : vertices) {
					if (check == key) {
						contains = true;
					}
					else {
					}
				}
				if (!contains) {
					vertices.push_back(key);
				}
			}
		}
		/*for (PxVec3 v : vertices) {
			char buf[4096], *p = buf;
			sprintf(buf, "result: %f, %f, %f\n", v[0], v[1], v[2]);
			OutputDebugString(buf);
		}*/

		int vertCount = vertices.size();
		PxVec3 *convexVerts = new PxVec3[vertCount];
		for (int i = 0; i < vertCount; i++) {
			convexVerts[i] = vertices[i];
		}

		PxConvexMeshDesc convexDesc;
		convexDesc.points.count = vertCount;
		convexDesc.points.stride = sizeof(PxVec3);
		convexDesc.points.data = convexVerts;
		convexDesc.flags = PxConvexFlag::eCOMPUTE_CONVEX;


		PxMaterial *mMaterial = gPhysics->createMaterial(0.5, 0.5, 0.5);

		PxDefaultMemoryOutputStream buf;
		PxConvexMeshCookingResult::Enum result;
		if (!gCooking->cookConvexMesh(convexDesc, buf, &result)) {
			std::cout << "Cooking Error" << std::endl;
			return;
		}
		PxDefaultMemoryInputData input(buf.getData(), buf.getSize());
		PxConvexMesh *convexMesh = gPhysics->createConvexMesh(input);
		PxRigidStatic *convexActor = gPhysics->createRigidStatic(PxTransform(PxVec3(0.0f, 0.0f, 0.0f)));
		PxShape *convexShape = PxRigidActorExt::createExclusiveShape(*convexActor, PxConvexMeshGeometry(convexMesh), *mMaterial);
		gScene->addActor(*convexActor);
		convexMesh->release();

		/*PxTriangleMeshDesc meshDesc;
		meshDesc.points.count = vertCount;
		meshDesc.points.stride = sizeof(PxVec3);
		meshDesc.points.data = convexVerts;
		meshDesc.triangles.count = md.meshes.size();
		meshDesc.triangles.stride = 3 * sizeof(PxU32);
		*/


	}
}

void Physx::releasePhysx() {
	controller->release();
	manager->release();
	gScene->release();
	gPhysics->release();
	gCooking->release();
	gPvd->release();
	gFoundation->release();
}