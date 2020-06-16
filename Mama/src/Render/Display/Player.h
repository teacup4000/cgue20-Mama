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

	glm::mat4 getModelMatrix();
	std::vector<AnimModel> getPlayerobject();
	glm::vec3 getPlayerPosition();
	glm::vec3 getPlayerFront();
	glm::vec3 getPlayerRight();

	void setFrontX(float x) { m_Front.x = x; }
	void setFrontY(float y) { m_Front.y = y; }
	void setFrontZ(float z) { m_Front.z = z; }

	void setPosition(glm::vec3 position) { m_Position = position; }
	void setRight(glm::vec3 right) { m_Right = right; }
	void setFront(glm::vec3 front) { m_Front = front; }
	void setFrontOld(glm::vec3 frontOld) { m_FrontOld = frontOld; }
	void setModel(bool setting) { m_ShowModel = setting; }
	void move(GLFWwindow *window, float& deltaTime);
	void Reset();
	void Update();

private:
	glm::vec3 m_StartPosition;
	glm::vec3 m_Position;
	glm::mat4 m_ModelMatrix;
	glm::mat4 m_Rotation;
	glm::vec3 m_MoveVector;
	glm::vec3 m_Right = glm::vec3(1, 0, 0);
	glm::vec3 m_Front = glm::vec3(0, 0, 1);
	glm::vec3 m_FrontOld = glm::vec3(0, 0, 0);

	float m_PlayerVelocity;
	float m_PlayerRunSpeed = 10.0f;
	float m_PlayerTurnSpeed = 60.0f;
	float m_PlayerCurrentSpeed = 0;
	float m_PlayerCurrentTurnSpeed = 0;
	bool m_IsPressed = false;
	float jumpHeight = m_Position.y;
	float lastJump = 0;
	float lastMoveTime = -1.0f;
	bool groundContact = true;
	bool jumped = false;
	bool peaked = false;
	bool positionSet = false;

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
			jumpHeight = m_Position.y;
		}

		//-----Physx chatacter controller movement------
		std::chrono::duration<float, std::milli> now = std::chrono::high_resolution_clock::now().time_since_epoch();
		float duration = lastMoveTime < 0 ? 0 : now.count() - lastMoveTime; // if first move then lastMove=-1 and no time passed, else calculate time passed
		lastMoveTime = now.count();


		physx::PxVec3 disp = physx::PxVec3(m_MoveVector.x, m_MoveVector.y, m_MoveVector.z);
		PxControllerCollisionFlags flags = m_Controller->move(disp, 0.01f, duration / 1000, NULL);

		int mask = 1 << 2;
		int maskedFlags = flags.operator uint8_t() & mask;
		int collisionDown = maskedFlags >> 2;

		if (collisionDown == 1) {
			groundContact = true;
		}
		else {
			groundContact = false;
		}

		m_Position.x = m_Controller->getFootPosition().x;
		m_Position.y = m_Controller->getFootPosition().y + 0.1;
		m_Position.z = m_Controller->getFootPosition().z;

		char buf[4096], *p = buf;
		sprintf(buf, "x %f\n", m_Position.x);
		OutputDebugString(buf);

		/*char buf[4096], *p = buf;
		sprintf(buf, "right %f %f %f\n", getPlayerRight()[0], getPlayerRight()[1], getPlayerRight()[2]);
		OutputDebugString(buf);*/

	}
};
