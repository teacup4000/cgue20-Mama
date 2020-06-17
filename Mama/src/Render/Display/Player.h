#pragma once
#define GLM_ENABLE_EXPERIMENTAL
#include <gtx/string_cast.hpp>

#include "Model.h"
#include "AnimModel.h"

#include "PxPhysics.h"
#include "PxPhysicsAPI.h"
#include "PhysX/PhysX.h"

#include <vector>
#include <iostream>
#include <chrono>

#include <GLFW\glfw3.h>

class Player
{
public:
	std::vector<AnimModel> m_PlayerModel;
	bool m_ShowModel;
	physx::PxController* m_Controller;

	Player() {}

	Player(glm::vec3 startPosition);
	void setPlayerModel(std::vector<AnimModel> playerObject);

	glm::mat4 getModelMatrix() { return m_ModelMatrix; }
	std::vector<AnimModel> getPlayerModel() { return m_PlayerModel; }
	glm::vec3 getPosition() { return m_Position; }
	glm::vec3 getFront() { return m_Front; }
	glm::vec3 getRight() { return m_Right; }

	void setFrontX(float x) { m_Front.x = x; }
	void setFrontY(float y) { m_Front.y = y; }
	void setFrontZ(float z) { m_Front.z = z; }

	void setPosition(glm::vec3 position) { m_Position = position; }
	void setRight(glm::vec3 right) { m_Right = right; }
	void setFront(glm::vec3 front) { m_Front = front; }
	void setFrontOld(glm::vec3 frontOld) { m_FrontOld = frontOld; }
	void setModel(bool setting) { m_ShowModel = setting; }
	void move(GLFWwindow *window, float& deltaTime);
	glm::vec3 moveCamera(GLFWwindow *window, float& deltaTime);
	void Reset();
	void Update();

private:
	glm::vec3 m_StartPosition;
	glm::vec3 m_Position;
	glm::mat4 m_ModelMatrix;
	glm::vec3 m_MoveVector;
	glm::vec3 m_Right = glm::vec3(1, 0, 0);
	glm::vec3 m_Front = glm::vec3(0, 0, 1);
	glm::vec3 m_FrontOld = glm::vec3(0, 0, 0);

	float m_PlayerVelocity;
	float m_PlayerRunSpeed = 5.0f;
	float jumpHeight = m_Position.y;
	float lastJump = 0;
	float lastMoveTime = -1.0f;

	bool groundContact = true;
	bool jumped = false;
	bool peaked = false;
	bool positionSet = false;
	bool cameraControl = false;

	void processKeyboard(GLFWwindow *window, float& deltaTime);
	
};
