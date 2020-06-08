#pragma once
#include <vector>
#include "Render/Display/Model.h"
#include "PxPhysicsAPI.h"
using namespace physx;

class Physx
{
public:
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
};