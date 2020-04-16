#include "Player.h"

extern Player player;

Player::Player(glm::vec3 startPosition)
{
	this->position = startPosition;
	this->modelMatrix = glm::mat4(1.0f);
	this->modelMatrix = glm::translate(modelMatrix, startPosition);
	//this->modelMatrix = glm::scale(modelMatrix, glm::vec3(0.5f, 0.5f, 0.5f));
}

void Player::setPlayerModel(std::vector<Model> playerObject)
{
	this->playerObject = playerObject;
}

void Player::rotate(float& movementSpeed, float& deltaTime)
{
	float velocity = movementSpeed * deltaTime;
	this->rotation = glm::rotate(rotation, glm::radians(20.0f*velocity), glm::vec3(0.0f, 1.0f, 0.0f));
}

glm::mat4 Player::getModelMatrix()
{
	return this->modelMatrix;
}

std::vector<Model> Player::getPlayerobject()
{
	return this->playerObject;
}

glm::vec3 Player::getPlayerPosition()
{
	return this->position;
}

void Player::processKeyboard(GLFWwindow *window, float& deltaTime)
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
		if (moveVector.y < 0.5f && position.y <= 0) {
			moveVector.y = velocity + 1;
		}

		std::cout << position.x << ", " << position.y << ", " << position.z << std::endl;
	}
	else if (glfwGetKey(window, GLFW_KEY_X) == GLFW_PRESS && !glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) {

		moveVector.y = -velocity;
		std::cout << "This is velocity" << position.y << std::endl;
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