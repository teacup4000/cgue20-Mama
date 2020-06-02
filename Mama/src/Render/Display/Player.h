#pragma once
#define GLM_ENABLE_EXPERIMENTAL
#include <gtx/string_cast.hpp>
#pragma once
#include "Model.h"
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
	void getDown(float& deltaTime);

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

	float lastMoveTime = -1.0f;


	virtual void rotate(float& movementSpeed, float& deltaTime);
	void processKeyboard(GLFWwindow *window, float& deltaTime)
	{
		m_PlayerVelocity = m_PlayerRunSpeed * deltaTime;
		m_MoveVector = glm::vec3(0);

		if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS && !glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		{
			m_MoveVector.z = -m_PlayerVelocity * m_Front.z;
			m_MoveVector.x = -m_PlayerVelocity * m_Front.x;
			std::cout << m_Position.x << ", " << m_Position.y << ", " << m_Position.z << std::endl;
		}
		else if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS && !glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
			m_MoveVector.z = m_PlayerVelocity * m_Front.z;
			m_MoveVector.x = m_PlayerVelocity * m_Front.x;
			std::cout << m_Position.x << ", " << m_Position.y << ", " << m_Position.z << std::endl;
		}
		if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS && !glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		{
			this->m_PlayerCurrentTurnSpeed = m_PlayerTurnSpeed;
			m_MoveVector.x = -m_PlayerVelocity * m_Right.x;
			m_MoveVector.z = -m_PlayerVelocity * m_Right.z;
			std::cout << m_Position.x << ", " << m_Position.y << ", " << m_Position.z << std::endl;
		}
		else if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS && !glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		{
			this->m_PlayerCurrentTurnSpeed = -m_PlayerTurnSpeed;
			m_MoveVector.x = m_PlayerVelocity * m_Right.x;
			m_MoveVector.z = m_PlayerVelocity * m_Right.z;

			std::cout << m_Position.x << ", " << m_Position.y << ", " << m_Position.z << std::endl;
		}
		else
		{
			this->m_PlayerCurrentTurnSpeed = 0;
		}

		if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS && !glfwGetKey(window, GLFW_KEY_X) == GLFW_PRESS)
		{
			//if (moveVector.y < 0.5f && position.y <= 0) {
			//	moveVector.y = velocity + 1;
			//}
			m_MoveVector.y = 1;
			std::cout << m_Position.x << ", " << m_Position.y << ", " << m_Position.z << std::endl;
		}
		else if (glfwGetKey(window, GLFW_KEY_X) == GLFW_PRESS && !glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) {
		
			m_MoveVector.y = -m_PlayerVelocity;
			std::cout <<"This is velocity" << m_Position.y  << std::endl;
		}
		
		else
		{
			this->m_PlayerCurrentTurnSpeed = 0;
			m_MoveVector.y = 0;
		}

		//m_Position += m_MoveVector;

		//TODO move fkn player model
		//-----Physx chatacter controller movement------
		//char buf[4096], *p = buf;
		std::chrono::duration<float, std::milli> now = std::chrono::high_resolution_clock::now().time_since_epoch(); //TODO Maybe don't use chrono?
		float duration = lastMoveTime < 0 ? 0 : now.count() - lastMoveTime; // if first move then lastMove=-1 and no time passed, else calculate time passed
		lastMoveTime = now.count();
		//sprintf(buf, "dura: %f\n", duration);
		//OutputDebugString(buf);
		physx::PxVec3 disp = physx::PxVec3(m_MoveVector.x, m_MoveVector.y - 0.5f, m_MoveVector.z); //TODO global variable for gravity
		controller->move(disp, 0.01f, duration / 1000, NULL); //TODO seconds or milliseconds?
		m_Position.x = controller->getPosition().x;
		m_Position.y = controller->getPosition().y;
		m_Position.z = controller->getPosition().z;
	}
	
	
};
