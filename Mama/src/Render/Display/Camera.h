#ifndef CAMERA_H
#define CAMERA_H
#define GLM_ENABLE_EXPERIMENTAL
#include <gtx/string_cast.hpp>
#pragma once
#include <glad/glad.h>

#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <GLFW\glfw3.h>
#include "Player.h"
#include "PhysX/PhysX.h"

//calculates the corresponding Euler Angles, Vectors and Matrices for use in OpenGL
class Camera
{
public:
	float m_Zoom;
	glm::vec3 m_Position;
	Camera() = default;

	Camera(Player* player, glm::vec3 currPosition = glm::vec3(0.0f, 0.0f, 0.0f));

	/** Returns the view matrix calculated using Euler Angles and the LookAt Matrix */
	glm::mat4 getViewMatrix();

	/** Processes input received from a mouse input system. Expects the offset value in both the x and y direction. */
	void processMouseMovement(float xoffset, float yoffset, GLboolean constrainPitch = true);

	/** Processes input received from a mouse scroll-wheel event. Only requires input on the vertical wheel-axis */
	void processMouseScroll(float yoffset);

	glm::mat4 getHealthBarMat(float remainingHealth);

	float getSensitivity() {
		return m_MouseSensitivity;
	}

	glm::vec3 getUp() { return m_Up; }
	glm::vec3 getFront() { return m_Front; }
	float getYaw() { return m_Yaw; }
	float getPitch() { return m_Yaw; }
	void setPhysx(Physx* physx) { m_physx = physx; }
	void loseScreen();
	void winScreen();
	void Reset();
	void move(GLFWwindow *window, float& deltaTime);
	void move(float xOffset, float yOffset);


private:

	//Camera Attributes
	glm::vec3 m_Front;
	glm::vec3 m_Up;
	glm::vec3 m_Right;
	glm::vec3 m_WorldUp;
	glm::vec3 m_EndPosition;
	//glm::vec3 cameraMove;
	//Euler Angles
	float m_Yaw, m_Pitch;
	//float m_LastX, m_LastY;
	float m_Distance;
	//Camera options
	float m_MovementSpeed;
	float m_MouseSensitivity;

	bool endScreen = false;
	bool cameraControl = false;

	Physx* m_physx;

	Player* m_Player;

	/** Calculates the front vector from the Camera's (updated) Euler Angles */
	void updateCameraVectors();
};

#endif