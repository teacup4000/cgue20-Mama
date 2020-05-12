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

	float getSensitivity() {
		return m_MouseSensitivity;
	}

private:

	//Camera Attributes
	glm::vec3 m_Front;
	glm::vec3 m_Up;
	glm::vec3 m_Right;
	glm::vec3 m_WorldUp;
	//glm::vec3 cameraMove;
	//Euler Angles
	float m_Yaw, m_Pitch;
	//float m_LastX, m_LastY;
	float m_Distance;
	//Camera options
	float m_MovementSpeed;
	float m_MouseSensitivity;
	

	Player* m_Player;


	/** Calculates the front vector from the Camera's (updated) Euler Angles */
	void updateCameraVectors();
};

#endif