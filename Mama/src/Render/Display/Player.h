#ifndef PLAYER_H
#define PLAYER_H
#define GLM_ENABLE_EXPERIMENTAL
#include <gtx/string_cast.hpp>
#pragma once
#include "Model.h"
#include <vector>
#include <iostream>
#include <GLFW\glfw3.h>

class Player
{
public:
	std::vector<Model> playerObject;
	glm::vec3 position;
	glm::mat4 modelMatrix;
	glm::mat4 rotation;
	glm::vec3 moveVector;
	glm::vec3 right = glm::vec3(1, 0, 0);
	glm::vec3 front = glm::vec3(0, 0, 1);

	float velocity;
	float run_speed = 10.0f;
	float turn_speed = 60.0f;
	float currentSpeed = 0;
	float currentTurnSpeed = 0;
	bool showModel;
	bool isPressed = false;

	Player() {}

	Player(glm::vec3 startPosition);
	virtual void setPlayerModel(std::vector<Model> playerObject);

	virtual glm::mat4 getModelMatrix();
	virtual std::vector<Model> getPlayerobject();
	virtual glm::vec3 getPlayerPosition();

	void move(GLFWwindow *window, float& deltaTime)
	{
		processKeyboard(window, deltaTime);
		rotate(currentTurnSpeed, deltaTime);

		this->modelMatrix = glm::translate(modelMatrix, moveVector);
	}
	void setModel(bool setting) {
		showModel = setting;
	}

	void getDown( float& deltaTime)
	{
		velocity = run_speed * deltaTime;
		moveVector = glm::vec3(0);
		//std::cout << "This is Y : " << position.y << std::endl;
		if( position.y > 15.0f) {
			moveVector.y -= velocity;
			position += moveVector; 
			this->modelMatrix = glm::translate(modelMatrix, moveVector);
			
		}
	}

private:

	virtual void rotate(float& movementSpeed, float& deltaTime);
	void processKeyboard(GLFWwindow *window, float& deltaTime)
	{
		velocity = run_speed * deltaTime;
		moveVector = glm::vec3(0);

		if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS && !glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		{
			moveVector.z = -velocity * front.z;
			moveVector.x = -velocity * front.x;
			std::cout << position.x << ", " << position.y << ", " << position.z << std::endl;
		}
		else if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS && !glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
			moveVector.z = velocity * front.z;
			moveVector.x = velocity * front.x;
			std::cout << position.x << ", " << position.y << ", " << position.z << std::endl;
		}
		if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS && !glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		{
			this->currentTurnSpeed = turn_speed;
			moveVector.x = -velocity * right.x;
			moveVector.z = -velocity * right.z;
			std::cout << position.x << ", " << position.y << ", " << position.z << std::endl;
		}
		else if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS && !glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		{
			this->currentTurnSpeed = -turn_speed;
			moveVector.x = velocity * right.x;
			moveVector.z = velocity * right.z;

			std::cout << position.x << ", " << position.y << ", " << position.z << std::endl;
		}
		else
		{
			this->currentTurnSpeed = 0;
		}

		if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS && !glfwGetKey(window, GLFW_KEY_X) == GLFW_PRESS)
		{
			//if (moveVector.y < 0.5f && position.y <= 0) {
			//	moveVector.y = velocity + 1;
			//}
			
			std::cout << position.x << ", " << position.y << ", " << position.z << std::endl;
		}
		else if (glfwGetKey(window, GLFW_KEY_X) == GLFW_PRESS && !glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) {
		
			moveVector.y = -velocity;
			std::cout <<"This is velocity" << position.y  << std::endl;
		}
		
		else
		{
			this->currentTurnSpeed = 0;
			moveVector.y = 0;
		}
		if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		{
			glfwSetWindowShouldClose(window, true);
		}
		if (glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS)
		{
			showModel = true;
			
		}
		/*	if (glfwGetKey(window, GLFW_KEY_1) == GLFW_RELEASE)
		{
			
			
		}*/

		position += moveVector;
	}
	
	
};

#endif