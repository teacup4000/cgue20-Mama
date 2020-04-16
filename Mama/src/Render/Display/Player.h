#define GLM_ENABLE_EXPERIMENTAL
#include <gtx/string_cast.hpp>
#pragma once
#include "Model.h"
#include <vector>
#include <iostream>
#include <GLFW\glfw3.h>

class Player
{
public://MEMBERS
	std::vector<Model> playerObject;
	glm::vec3 position;
	glm::mat4 modelMatrix;
	glm::mat4 rotation;
	glm::vec3 moveVector;
	glm::vec3 right = glm::vec3(1, 0, 0);
	glm::vec3 front = glm::vec3(0, 0, 1);

	bool showModel;

public://FUNCTIONS

	Player() = default;
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
		std::cout << "This is Y : " << position.y << std::endl;
		if( position.y > 0.0f) {
			moveVector.y -= velocity;
			position += moveVector; 
			this->modelMatrix = glm::translate(modelMatrix, moveVector);
		}
	}

private:
	float velocity;
	float run_speed = 10.0f;
	float turn_speed = 60.0f;
	float currentSpeed = 0;
	float currentTurnSpeed = 0;
	bool isPressed = false;

private: //FUNCTIONS
	virtual void rotate(float& movementSpeed, float& deltaTime);
	void processKeyboard(GLFWwindow *window, float& deltaTime);
};

