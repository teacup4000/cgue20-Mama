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
	desc.contactOffset = 0.1f; //controller skin within which contacts generated
	desc.stepOffset = 0.01; //max obstacle height the character can climb
	desc.slopeLimit = cosf(glm::radians(45.0f)); // max slope the character can walk
	desc.radius = 0.3f; //radius of the capsule
	desc.height = 0.5f; //height of the controller
	desc.upDirection = PxVec3(0, 1, 0); // Specifies the 'up' direction
	desc.material = gPhysics->createMaterial(0.5f, 0.5f, 0.1f); // material defines physical properties like friction, bounciness etc. - see PhysX materials
	controller = manager->createController(desc);

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

void Physx::createTrigger(PxVec3 position, PxVec3 size, TriggerType type) {
	
	PxMaterial *mMaterial = gPhysics->createMaterial(0.5, 0.5, 0.5);
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
		OutputDebugString("How did you even...?\n");
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
	PxRaycastBuffer hit;

	bool status = gScene->raycast(playerPos, unitDir, length, hit);

	if (status) {
		PxShape *cShapes[1];
		hit.block.actor->getShapes(cShapes, 1, 0);
		PxShape *cShape = cShapes[0];
		PxShapeFlags flags = cShape->getFlags();

		int mask = 1 << 2;
		int maskedFlags = flags.operator uint8_t() & mask;
		int triggerFlag = maskedFlags >> 2;
		
		//hit a trigger or the player character -> ignore
		if (triggerFlag == 1 || hit.block.actor == controller->getActor()) {
			return length;
		}
		else {
			return hit.block.distance;
		}
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