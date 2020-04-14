#ifndef CAMERA_H
#define CAMERA_H
#define GLM_ENABLE_EXPERIMENTAL
#include <gtx/string_cast.hpp>
#pragma once
#include <glad/glad.h>

#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <GLFW\glfw3.h>
#include "player.h"

//possible options for camera movement
enum Camera_Movement {
	FORWARD,
	BACKWARD,
	LEFT,
	RIGHT
};

//default settings
const float PLAYER_DISTANCE = 3.0f;
const float YAW = -90.0f;
const float PITCH = 0.0f;
const float SPEED = 2.5f;
const float SENSITIVITY = 0.1f;
const float ZOOM = 45.0f;

//calculates the corresponding Euler Angles, Vectors and Matrices for use in OpenGL
class Camera
{
public:
	//Camera Attributes
	glm::vec3 position;
	glm::vec3 front;
	glm::vec3 up;
	glm::vec3 right;
	glm::vec3 worldUp;
	//glm::vec3 cameraMove;
	//Euler Angles
	float yaw;
	float pitch;
	//Camera options
	float movementSpeed;
	float mouseSensitivity;
	float zoom;

	Player* player;


	Camera(Player* player,  glm::vec3 currPosition = glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3 currUp = glm::vec3(0.0f, 1.0f, 0.0f), float currYaw = YAW, float currPitch = PITCH) : front(glm::vec3(0.0f, 0.0f, -1.0f)), movementSpeed(SPEED), mouseSensitivity(SENSITIVITY), zoom(ZOOM)
	{
		position = currPosition;
		worldUp = currUp;
		yaw = currYaw;
		pitch = currPitch;
		this->player = player;
		updateCameraVectors();
	}

	Camera(Player* player, float posX, float posY, float posZ, float upX, float upY, float upZ, float currYaw, float currPitch) : front(glm::vec3(0.0f, 0.0f, -1.0f)), movementSpeed(SPEED), mouseSensitivity(SENSITIVITY), zoom(ZOOM)
	{
		position = glm::vec3(posX, posY, posZ);
		worldUp = glm::vec3(upX, upY, upZ);
		yaw = currYaw;
		pitch = currPitch;
		this->player = player;
		updateCameraVectors();
	}

	/** Returns the view matrix calculated using Euler Angles and the LookAt Matrix */
	glm::mat4 getViewMatrix();

	/** Processes input received from any keyboard-like input system. Accepts input parameter in the form of camera defined ENUM (to abstract it from windowing systems) */
	//void processKeyboard(Camera_Movement direction, float deltaTime);

	/** Processes input received from a mouse input system. Expects the offset value in both the x and y direction. */
	void processMouseMovement(float xoffset, float yoffset, GLboolean constrainPitch = true);

	/** Processes input received from a mouse scroll-wheel event. Only requires input on the vertical wheel-axis */
	void processMouseScroll(float yoffset);

private:
	/** Calculates the front vector from the Camera's (updated) Euler Angles */
	void updateCameraVectors();
};

#endif