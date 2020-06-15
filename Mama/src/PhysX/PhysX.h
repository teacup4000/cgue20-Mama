#pragma once
#include <vector>
#include "Render/Display/Model.h"
#include "PxPhysicsAPI.h"
#include "PxSimulationEventCallback.h"
#include "../Core/Game.h"
using namespace physx;

class Physx : public PxSimulationEventCallback
{

public:

	enum TriggerType
	{
		TRAP,
		MEAT,
		MOMMY
	};
	
	void simulate() {
		gScene->simulate(1.0f / 60.0f);
		gScene->fetchResults(true);
	}


	PxController* getController() { return controller; };
	//initializes Physx
	void initPhysx();
	//Releases all Physx Components
	void releasePhysx();
	//Cooks the Collision Models for Physx
	void createModels(std::vector<Model> models);
	//create a Trigger Box of Size size at Position position
	void createTrigger(PxVec3 position, PxVec3 size, TriggerType type);
	
	//PxSimulationEventCallback functions
	virtual void onContact(const PxContactPairHeader& pairHeader, const PxContactPair* pairs, PxU32 nbPairs) { OutputDebugString("CONTACT\n"); }
	virtual void onTrigger(PxTriggerPair* pairs, PxU32 count);
	virtual void onConstraintBreak(PxConstraintInfo*, PxU32) {}
	virtual void onWake(PxActor**, PxU32) {}
	virtual void onSleep(PxActor**, PxU32) {}
	virtual void onAdvance(const PxRigidBody*const*, const PxTransform*, const PxU32) {}

	TriggerType getLastTriggerType() { return lastTriggerType; }
	PxActor* getLastTrigger() { return lastTrigger; }
	PxActor* getNewTrigger() { return newTrigger; }
	float getLastTriggerTime() { return lastTriggerTime; }
	float getNewTriggerTime() { return newTriggerTime; }

	void setGame(Game* game) { m_Game = game; }

private:
	PxDefaultErrorCallback gDefaultErrorCallback;
	PxDefaultAllocator gDefaultAllocatorCallback;
	PxFoundation* gFoundation = nullptr;
	PxCooking *gCooking = nullptr;
	PxPvd *gPvd = nullptr;
	PxPhysics *gPhysics = nullptr;
	PxScene *gScene = nullptr;
	PxController *controller = nullptr;
	PxControllerManager *manager = nullptr;

	TriggerType lastTriggerType;
	PxActor *lastTrigger;
	PxActor *newTrigger;
	float lastTriggerTime;
	float newTriggerTime;

	std::vector<PxActor*> traps;
	std::vector<PxActor*> meat;
	PxRigidStatic *mommy = nullptr;

	Game* m_Game;
};