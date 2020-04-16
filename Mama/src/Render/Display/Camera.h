#define GLM_ENABLE_EXPERIMENTAL
#include <gtx/string_cast.hpp>
#pragma once
#include <glad/glad.h>

#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <GLFW\glfw3.h>
#include "Player.h"

//Calculates the corresponding Euler Angles, Vectors and Matrices for use in OpenGL
class Camera
{
public:
	float m_Zoom;
	glm::vec3 m_Position;

	Camera() = default;
	Camera(Player* player, glm::vec3 currPosition = glm::vec3(0.0f, 0.0f, 0.0f));

	// Returns the view matrix calculated using Euler Angles and the LookAt Matrix 
	glm::mat4 getViewMatrix();
	void processMouseMovement(float xoffset, float yoffset, GLboolean constrainPitch = true);
	void processMouseScroll(float yoffset);

private: //MEMBERS
	//CAMERA ATTRIBUTES
		glm::vec3 m_Front;
		glm::vec3 m_Up;
		glm::vec3 m_Right;
		glm::vec3 m_WorldUp;
		//glm::vec3 cameraMove;

	//EULER ANGLES
		float m_Yaw, m_Pitch;

	//DISTANCE
	float m_Distance;

	//ADDITIONAL OPTIONS
	float m_MovementSpeed;
	float m_MouseSensitivity;
	
	Player* m_Player;

private: //FUNCTIONS
	// Calculates the front vector from the Camera's (updated) Euler Angles 
	void updateCameraVectors();
};
