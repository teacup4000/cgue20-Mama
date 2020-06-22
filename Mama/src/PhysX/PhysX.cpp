#include "stdafx.h"
#include "Physx.h"
#include <chrono>
#include <math.h>

void Physx::initPhysx()
{
	//-----------------------------CREATE FOUNDATION--------------------------------
	gFoundation = PxCreateFoundation(PX_PHYSICS_VERSION, gDefaultAllocatorCallback,
		gDefaultErrorCallback);
	if (!gFoundation)
		std::cout << "PxCreateFoundation failed!" << std::endl;
	
	std::cout << "Physx Foundation created!" << std::endl;
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

	std::cout << "Physx Main Object created!" << std::endl;
	//--------------------------------CREATE Cooking---------------------------------------


	gCooking = PxCreateCooking(PX_PHYSICS_VERSION, *gFoundation, PxCookingParams(scale));
	if (!gCooking)
		std::cout << "PxCreateCooking failed!" << std::endl;

	std::cout << "Physx Cooking created!" << std::endl;
	//--------------------------------CREATE SCENE---------------------------------------
	PxSceneDesc sceneDesc(gPhysics->getTolerancesScale());
	sceneDesc.gravity = PxVec3(0.0f, -9.8f, 0.0f);
	sceneDesc.cpuDispatcher = PxDefaultCpuDispatcherCreate(1);
	sceneDesc.filterShader = PxDefaultSimulationFilterShader;
	gScene = gPhysics->createScene(sceneDesc);
	gScene->setSimulationEventCallback(this);

	//--------------------------------CREATE PVD Client---------------------------------------
	PxPvdSceneClient *pvdClient = gScene->getScenePvdClient();
	if (pvdClient)
	{
		pvdClient->setScenePvdFlag(PxPvdSceneFlag::eTRANSMIT_CONSTRAINTS, true);
		pvdClient->setScenePvdFlag(PxPvdSceneFlag::eTRANSMIT_CONTACTS, true);
		pvdClient->setScenePvdFlag(PxPvdSceneFlag::eTRANSMIT_SCENEQUERIES, true);
	}


	//----------------------------------------------------------------------------------------
	//-----------------------------CHARACTER CONTROLLER---------------------------------------
	//----------------------------------------------------------------------------------------

	manager = PxCreateControllerManager(*gScene);
	PxCapsuleControllerDesc desc;
	manager->setOverlapRecoveryModule(true);
	desc.position = PxExtendedVec3(-9.26916f, 14.4f, -15.9479f); //initial position
	desc.contactOffset = 0.001f; //controller skin within which contacts generated
	desc.stepOffset = 0.01; //max obstacle height the character can climb
	desc.slopeLimit = cosf(glm::radians(45.0f)); // max slope the character can walk
	desc.radius = 0.1f; //radius of the capsule
	desc.height = 0.3f; //height of the controller
	desc.upDirection = PxVec3(0, 1, 0); // Specifies the 'up' direction
	desc.material = gPhysics->createMaterial(0.5f, 0.5f, 0.1f); // material defines physical properties like friction, bounciness etc. - see PhysX materials
	controller = manager->createController(desc);
	std::cout << "Player Character Controller created!" << std::endl;

}

void Physx::createModels(std::vector<Model> models) {
	
	for (Model md : models) {
		PxVec3 *verts = new PxVec3[md.meshes[0].vertices.size()];
		int i = 0;
		for (Vertex v : md.meshes[0].vertices) {
			verts[i] = PxVec3(v.position.x, v.position.y, v.position.z);
			i++;
		}
		int vertsCount = i;

		PxU32 *indices = new PxU32[md.meshes[0].indices.size()];
		i = 0;
		for (GLuint in : md.meshes[0].indices) {
			indices[i] = in;
			i++;
		}
		int indexCount = i;
		
		PxTriangleMeshDesc meshDesc;
		meshDesc.points.count = vertsCount;
		meshDesc.points.stride = sizeof(PxVec3);
		meshDesc.points.data = verts;
		meshDesc.triangles.count = md.getFaceCount();
		meshDesc.triangles.stride = 3 * sizeof(PxU32);
		meshDesc.triangles.data = indices;

		PxMaterial *mMaterial = gPhysics->createMaterial(0.5, 0.5, 0.5);
		
		PxDefaultMemoryOutputStream buf;
		PxTriangleMeshCookingResult::Enum result;
		if (!gCooking->cookTriangleMesh(meshDesc, buf, &result)) {
			std::cout << "Cooking Error" << std::endl;
			return;
		}
		PxDefaultMemoryInputData input(buf.getData(), buf.getSize());
		PxTriangleMesh* triangleMesh = gPhysics->createTriangleMesh(input);
		PxRigidStatic *triangleActor = gPhysics->createRigidStatic(PxTransform(PxVec3(0.0f)));
		PxShape* triangleShape = PxRigidActorExt::createExclusiveShape(*triangleActor, PxTriangleMeshGeometry(triangleMesh), *mMaterial);
		gScene->addActor(*triangleActor);
		std::cout << "Physx Static Model created!" << std::endl;
			
		triangleMesh->release();
		
	}
}

void Physx::createDynamic(PxVec3 position, DynamicType type, PxVec3 dimensions, int mass) {
	PxMaterial* mMaterial;
	PxRigidDynamic* actor;
	PxShape* shape;

	switch (type)
	{
	case Physx::SPHERE:
		mMaterial = gPhysics->createMaterial(1.0f, 0.5f, 0.1f);
		actor = gPhysics->createRigidDynamic(PxTransform(position));
		shape = PxRigidActorExt::createExclusiveShape(*actor, PxSphereGeometry(dimensions.x), *mMaterial);
		PxRigidBodyExt::setMassAndUpdateInertia(*actor, mass);
		gScene->addActor(*actor);
		dynamicObjects.push_back(actor);
		break;
	case Physx::BOX:
		break;
	case Physx::CAPSULE:
		mMaterial = gPhysics->createMaterial(1.0f, 0.5f, 0.1f);
		actor = gPhysics->createRigidDynamic(PxTransform(position));
		shape = PxRigidActorExt::createExclusiveShape(*actor, PxCapsuleGeometry(dimensions.x, dimensions.y), *mMaterial);
		PxRigidBodyExt::setMassAndUpdateInertia(*actor, mass);
		gScene->addActor(*actor);
		dynamicObjects.push_back(actor);
		break;
	default:
		break;
	}
}

void Physx::createTrigger(PxVec3 position, PxVec3 size, TriggerType type) {
	
	PxMaterial *mMaterial = gPhysics->createMaterial(0.5, 0.5, 0.1);
	PxRigidStatic *trigger;
	PxTransform triggerPos(position);
	PxBoxGeometry triggerGeometry(size);
	PxShape *triggerShape = gPhysics->createShape(triggerGeometry, *mMaterial);
	trigger = gPhysics->createRigidStatic(triggerPos);
	triggerShape->setFlag(PxShapeFlag::eSIMULATION_SHAPE, false);
	triggerShape->setFlag(PxShapeFlag::eTRIGGER_SHAPE, true);
	trigger->attachShape(*triggerShape);
	gScene->addActor(*trigger);
	
	switch (type)
	{
	case TRAP:
		traps.push_back(trigger);
		break;
	case MEAT:
		meat.push_back(trigger);
		meatPos.push_back(position);
		break;
	case MOMMY:
		if (!mommy) {
			mommy = trigger;
		}
		else {
			std::cout << "Error: Multiple Mommies detected." << std::endl;
		}
		break;
	case BOY:
		if (boy) {
			gScene->removeActor(*boy);
		}
			boy = trigger;
	
		break;
	default:
		std::cout << "Error: Unknown Triggertype!" << std::endl;
		break;
	}
}

void Physx::onTrigger(PxTriggerPair* pairs, PxU32 count) {
	if (!m_Game->isPaused()) {
		for (PxU32 i = 0; i < count; i++) {

			bool isMeat = false;
			bool isEaten = false;
			int meatCount = 0;

			std::chrono::duration<float, std::milli> now = std::chrono::high_resolution_clock::now().time_since_epoch();
			lastTriggerTime = newTriggerTime;
			newTriggerTime = now.count() / 1000;

			lastTrigger = newTrigger;
			newTrigger = pairs[i].triggerActor;

			if (pairs[i].otherActor == controller->getActor()) {
				if (lastTrigger != newTrigger || newTriggerTime - lastTriggerTime > 1) {
					if (pairs[i].triggerActor == mommy) {
						m_Game->Win();
					}
					else if (pairs[i].triggerActor == boy) {
						//more time to recover from trample damage
						if (newTriggerTime - lastTriggerTime > 2) {
							m_Game->TrampleDamage();
						}
					}
					else {
						for (PxActor *a : meat) {
							if (pairs[i].triggerActor == a) {
								for (PxActor* e : eaten) {
									//if meat already eaten -> ignore
									if (e == a) {
										eatenPos.push_back(meatPos[meatCount]);
										isEaten = true;
										break;
									}
								}
								if (!isEaten) {
									isMeat = true;
									//only eat if we lack health
									if (m_Game->getLife() < 100) {
										eaten.push_back(a);
										m_Game->GainLife();
									}
								}
								break;
							}
							meatCount++;
						}
						if (!isMeat) {
							for (PxActor *a : traps) {
								if (pairs[i].triggerActor == a) {
									m_Game->ReceiveDamage();

									break;
								}
							}
						}
					}
				}
			}
		}
	}
}

float Physx::checkCamera(PxVec3 cameraPos, PxVec3 playerPos) {
	
	PxVec3 unitDir(cameraPos - playerPos);
	float length = sqrt(pow(unitDir.x, 2) + pow(unitDir.y, 2) + pow(unitDir.z, 2));
	unitDir = unitDir.getNormalized();
	const PxU32 bufferSize = 256;
	PxRaycastHit hitBuffer[256];
	PxRaycastBuffer hits(hitBuffer, bufferSize);

	bool status = gScene->raycast(playerPos, unitDir, length, hits);

	if (status) {
		for (int i = 0; i < hits.getNbTouches(); i++) {

			PxShape *cShapes[1];
			hits.getTouches()[i].actor->getShapes(cShapes, 1, 0);
			PxShape *cShape = cShapes[0];
			PxShapeFlags flags = cShape->getFlags();

			int mask = 1 << 2;
			int maskedFlags = flags.operator uint8_t() & mask;
			int triggerFlag = maskedFlags >> 2;

			//check if hit is not trigger and player character
			if (triggerFlag == 0 && hits.getTouches()[i].actor != controller->getActor()) {
				return hits.getTouches()[i].distance;
			}
		}
		return length;
	}
	else {
		return length;
	}

}

bool Physx::checkMeat(PxVec3 position) {
	for (PxVec3 v : eatenPos) {
		if (position == v) {
			return true;
		}
	}
	return false;
}

void Physx::Reset() {
	std::chrono::duration<float, std::milli> now = std::chrono::high_resolution_clock::now().time_since_epoch();
	newTriggerTime = now.count() /1000;
	eaten.clear();
	eatenPos.clear();
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