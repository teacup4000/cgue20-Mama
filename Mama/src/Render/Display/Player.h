#pragma once
#define GLM_ENABLE_EXPERIMENTAL
#include <gtx/string_cast.hpp>
#pragma once
#include "Model.h"
#include "../../Core/Physx.h"
#include "PxPhysics.h"
#include "PxPhysicsAPI.h"
#include <vector>
#include <iostream>
#include <GLFW\glfw3.h>
#include <chrono>

class Player
{
public:
	std::vector<Model> m_PlayerModel;
	bool m_ShowModel;

	Player() {}

	Player(glm::vec3 startPosition);
	void setPlayerModel(std::vector<Model> playerObject);

	glm::mat4 getModelMatrix();
	std::vector<Model> getPlayerobject();
	glm::vec3 getPlayerPosition();
	glm::vec3 getPlayerFront();
	glm::vec3 getPlayerRight();

	physx::PxController *controller;

	void setFrontX(float x) { m_Front.x = x; }
	void setFrontY(float y) { m_Front.y = y; }
	void setFrontZ(float z) { m_Front.z = z; }

	void setRight(glm::vec3 right) { m_Right = right; }
	void setFront(glm::vec3 front) { m_Front = front; }
	void setModel(bool setting) { m_ShowModel = setting; }
	void move(GLFWwindow *window, float& deltaTime);
	void updatePosition();

private:
	glm::vec3 m_Position;
	glm::mat4 m_ModelMatrix;
	glm::mat4 m_Rotation;
	glm::vec3 m_MoveVector;
	glm::vec3 m_Right = glm::vec3(1, 0, 0);
	glm::vec3 m_Front = glm::vec3(0, 0, 1);

	float m_PlayerVelocity;
	float m_PlayerRunSpeed = 10.0f;
	float m_PlayerTurnSpeed = 60.0f;
	float m_PlayerCurrentSpeed = 0;
	float m_PlayerCurrentTurnSpeed = 0;
	bool m_IsPressed = false;
	float jumpHeight = m_Position.y;
	float lastJump = 0;
	bool groundContact = true;
	bool jumped = false;
	bool peaked = false;

	float lastMoveTime = -1.0f;


	virtual void rotate(float& movementSpeed, float& deltaTime);
	void processKeyboard(GLFWwindow *window, float& deltaTime)
	{
		m_PlayerVelocity = m_PlayerRunSpeed * deltaTime;
		m_MoveVector = glm::vec3(0);
		m_MoveVector.y = m_MoveVector.y - GRAVITY * deltaTime * 250;

		if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS && !glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		{
			m_MoveVector.z = -m_PlayerVelocity * m_Front.z;
			m_MoveVector.x = -m_PlayerVelocity * m_Front.x;
		}
		else if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS && !glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
			m_MoveVector.z = m_PlayerVelocity * m_Front.z;
			m_MoveVector.x = m_PlayerVelocity * m_Front.x;
		}
		if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS && !glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		{
			this->m_PlayerCurrentTurnSpeed = m_PlayerTurnSpeed;
			m_MoveVector.x = -m_PlayerVelocity * m_Right.x;
			m_MoveVector.z = -m_PlayerVelocity * m_Right.z;
		}
		else if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS && !glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		{
			this->m_PlayerCurrentTurnSpeed = -m_PlayerTurnSpeed;
			m_MoveVector.x = m_PlayerVelocity * m_Right.x;
			m_MoveVector.z = m_PlayerVelocity * m_Right.z;

		}
		else
		{
			this->m_PlayerCurrentTurnSpeed = 0;
		}

		if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
		{
			std::chrono::duration<float, std::milli> now = std::chrono::high_resolution_clock::now().time_since_epoch() / 1000;
			if (!jumped && groundContact && (lastJump == 0 || now.count() - lastJump > 1))
			{
				m_MoveVector.y = 0.025f * deltaTime * 250;
				jumped = true;
				std::chrono::duration<float, std::milli> last = std::chrono::high_resolution_clock::now().time_since_epoch();
				lastJump = last.count() / 1000;
			}
			else if (!peaked && jumped && m_Position.y - jumpHeight < 1 && m_Position.y - jumpHeight >= 0) {
				m_MoveVector.y = 0.025f * deltaTime * 250;
			}
			else if (!peaked && jumped && m_Position.y - jumpHeight >= 1) {
				peaked = true;
			}
		}
		
		else
		{	
			if (jumped && groundContact) {
				jumped = false;
				peaked = false;
			}
			this->m_PlayerCurrentTurnSpeed = 0;
			//m_MoveVector.y = 0;
			jumpHeight = m_Position.y;
		}

		//-----Physx chatacter controller movement------
		std::chrono::duration<float, std::milli> now = std::chrono::high_resolution_clock::now().time_since_epoch();
		float duration = lastMoveTime < 0 ? 0 : now.count() - lastMoveTime; // if first move then lastMove=-1 and no time passed, else calculate time passed
		lastMoveTime = now.count();
		
		
		physx::PxVec3 disp = physx::PxVec3(m_MoveVector.x, m_MoveVector.y, m_MoveVector.z);
		PxControllerCollisionFlags flags = controller->move(disp, 0.01f, duration / 1000, NULL);
		
		int mask = 1 << 2;
		int maskedFlags = flags.operator uint8_t() & mask;
		int collisionDown = maskedFlags >> 2;

		if (collisionDown == 1) {
			groundContact = true;
		}
		else {
			groundContact = false;
		}

		m_Position.x = controller->getFootPosition().x;
		m_Position.y = controller->getFootPosition().y + 0.1;
		m_Position.z = controller->getFootPosition().z;
		
		char buf[4096], *p = buf;
		sprintf(buf, "cntr %f %f %f\n", controller->getPosition()[0], controller->getPosition()[1], controller->getPosition()[2]);
		OutputDebugString(buf);
		sprintf(buf, "feet %f %f %f\n", controller->getFootPosition()[0], controller->getFootPosition()[1], controller->getFootPosition()[2]);
		OutputDebugString(buf);
	}
	
	
};
