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
		MOMMY,
		BOY
	};

	enum DynamicType
	{
		SPHERE,
		BOX,
		CAPSULE
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
	//Cooks the Collision Models for Physx; dynamic specifies if Object is dynamic, false per default
	void createModels(std::vector<Model> models, bool dynamic = false);
	//creates a dynamic object of Type type at Position position with the Dimensions dimensions(radius/x, height/y, z) and the Mass mass
	void createDynamic(PxVec3 position, DynamicType type, PxVec3 dimensions, int mass);
	//create a Trigger Box of Size size at Position position
	void createTrigger(PxVec3 position, PxVec3 size, TriggerType type);
	//checks if the camera colides with something and gives out collision distance
	float checkCamera(PxVec3 cameraPos, PxVec3 playerPos);


	//PxSimulationEventCallback functions
	virtual void onContact(const PxContactPairHeader& pairHeader, const PxContactPair* pairs, PxU32 nbPairs) {}
	virtual void onTrigger(PxTriggerPair* pairs, PxU32 count);
	virtual void onConstraintBreak(PxConstraintInfo*, PxU32) {}
	virtual void onWake(PxActor**, PxU32) {}
	virtual void onSleep(PxActor**, PxU32) {}
	virtual void onAdvance(const PxRigidBody*const*, const PxTransform*, const PxU32) {}

	PxActor* getLastTrigger() { return lastTrigger; }
	PxActor* getNewTrigger() { return newTrigger; }
	float getLastTriggerTime() { return lastTriggerTime; }
	float getNewTriggerTime() { return newTriggerTime; }
	std::vector<PxRigidDynamic*> getDynamicObjects() { return dynamicObjects; }
	//checks meat has already been eaten; returns true if so
	bool checkMeat(PxVec3 position);
	void setGame(Game* game) { m_Game = game; }
	void Reset();

private:
	PxDefaultErrorCallback gDefaultErrorCallback;
	PxDefaultAllocator gDefaultAllocatorCallback;
	PxFoundation* gFoundation = nullptr;
	PxCooking* gCooking = nullptr;
	PxPvd* gPvd = nullptr;
	PxPhysics* gPhysics = nullptr;
	PxScene* gScene = nullptr;
	PxController* controller = nullptr;
	PxControllerManager* manager = nullptr;
	

	PxActor* lastTrigger;
	PxActor* newTrigger;
	float lastTriggerTime;
	float newTriggerTime;

	std::vector<PxActor*> traps;
	std::vector<PxActor*> eaten;
	std::vector<PxActor*> meat;
	std::vector<PxVec3> meatPos;
	std::vector<PxVec3> eatenPos;
	std::vector<PxRigidDynamic*> dynamicObjects;
	PxRigidStatic* mommy = nullptr;
	PxRigidStatic* boy = nullptr;

	Game* m_Game;
};