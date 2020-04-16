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