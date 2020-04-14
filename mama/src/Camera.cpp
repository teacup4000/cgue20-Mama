#include "Camera.h"
#include <gtx/transform.hpp>
#include <iostream>

extern float lastX; //mouseOffset since last Frame 
extern float lastY;
extern Camera camera;



bool firstMouse = true;
bool cursor = false;
extern bool fullscreen = false;


void setFullscreen(bool& fullscreen);



//----------------------------------------------------------------------Camera funktions------------------------------------------------------------------------------------------
glm::mat4 Camera::getViewMatrix()
{
	this->position = this->player->position;
	this->position -= this->front * PLAYER_DISTANCE;
	return glm::lookAt(this->position, this->position + this->front, this->worldUp);
}

void Camera::processMouseScroll(float yOffset)
{
	if (zoom >= 1.0f && zoom <= 45.0f)
		zoom -= yOffset;
	if (zoom <= 1.0f)
		zoom = 1.0f;
	if (zoom >= 45.0f)
		zoom = 45.0f;
}

void Camera::updateCameraVectors()
{
	
	// Calculate the new Front vector
	glm::vec3 frontNew;
	frontNew.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
	frontNew.y = sin(glm::radians(pitch));
	frontNew.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
	front = glm::normalize(frontNew);


	// Also re-calculate the Right and Up vector
	right = glm::normalize(glm::cross(front, worldUp));  // Normalize the vectors, because their length gets closer to 0 the more you look up or down which results in slower movement.
	up = glm::normalize(glm::cross(right, front));
	this->player->right = this->right;
	this->player->front = -this->front;
	this->player->front.y = 0;
	this->player->front = glm::normalize(this->player->front);
}

void Camera::processMouseMovement(float xOffset, float yOffset, GLboolean constrainPitch)
{
	//this->position = this->player->position;
	xOffset *= this->mouseSensitivity;
	yOffset *= this->mouseSensitivity;

	yaw += xOffset;
	pitch += yOffset;
	
	/* Make sure that when pitch is out of bounds, screen doesn't get flipped */
	if (constrainPitch)
	{
		if (pitch > 89.0f)
			pitch = 89.0f;
		if (pitch < -89.0f)
			pitch = -89.0f;
	}



	this->position = this->player->position;

	/* Update Front, Right and Up Vectors using the updated Euler angles */
	this->updateCameraVectors();

	this->position -= this->front * PLAYER_DISTANCE;
}

/** process movement and termination */
/*void processInput(GLFWwindow* window, float& deltaTime)
{
	float cameraSpeed = 20.0f * deltaTime;

	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
	{
		camera.processKeyboard(FORWARD, deltaTime);
		std::cout << camera.position.x << ", " << camera.position.y << ", " << camera.position.z << std::endl;
	}
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
	{
		camera.processKeyboard(BACKWARD, deltaTime);
		std::cout << camera.position.x << ", " << camera.position.y << ", " << camera.position.z << std::endl;
	}
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
	{
		camera.processKeyboard(LEFT, deltaTime);
		std::cout << camera.position.x << ", " << camera.position.y << ", " << camera.position.z << std::endl;
	}
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
	{
		camera.processKeyboard(RIGHT, deltaTime);
		std::cout << camera.position.x << ", " << camera.position.y << ", " << camera.position.z << std::endl;
	}
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, true);
	}
}*/

/*void Camera::processKeyboard(Camera_Movement direction, float deltaTime)
{
	float velocity = movementSpeed * deltaTime;

	if (direction == FORWARD)
	{
		this->position += this->front * velocity;
	}
	if (direction == BACKWARD)
	{
		this->position -= this->front * velocity;
	}
	if (direction == LEFT)
	{
		this->position -= this->right * velocity;
	}
	if (direction == RIGHT)
	{
		this->position += this->right * velocity;
	}
}*/

void key_callback(GLFWwindow* window, int key, int code, int action, int modifers)
{
	/* Layers are dependend to the "doTheLayer" function in UI.cpp. If changing something in this file, make sure you change it there too! */
	if (key == GLFW_KEY_F1 && action == GLFW_PRESS)
	{
		fullscreen = !fullscreen;
		setFullscreen(fullscreen);
	}

}

/** process input from Mouse buttons */
void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
	if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_PRESS)
	{
		cursor = !cursor;
		if (cursor)
		{
			std::cout << "cursor true" << std::endl;
			glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
		}
		else
		{
			std::cout << "cursor false" << std::endl;
			glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
		}
	}
}

/** whenever the window size changes this function is called */
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

/** whenever the mouse moves, this function is called */
void mouse_callback(GLFWwindow* window, double xPos, double yPos)
{
	if (firstMouse)
	{
		lastX = xPos;
		lastY = yPos;
		firstMouse = false;
	}
	float xOffset = xPos - lastX;
	float yOffset = lastY - yPos;

	lastX = xPos;
	lastY = yPos;

	camera.processMouseMovement(xOffset, yOffset);
}

/** whenever the mouse scroll wheel scrolls, this function is called */
void scroll_callback(GLFWwindow* window, double xOffset, double yOffset)
{
	std::cout << yOffset << std::endl;
	camera.processMouseScroll(yOffset);
}