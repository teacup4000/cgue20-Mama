#include "stdafx.h"
#include "Player.h"

extern Player player;

Player::Player(glm::vec3 startPosition)
{
	std::cout << "Startposition is " << startPosition.x << " " <<startPosition.y << " " << startPosition.z << std::endl;
	m_Position = startPosition;
	m_ModelMatrix = glm::mat4(1.0f);
	m_ModelMatrix = glm::translate(m_ModelMatrix, startPosition);
	//this->modelMatrix = glm::scale(modelMatrix, glm::vec3(0.5f, 0.5f, 0.5f));
}

void Player::setPlayerModel(std::vector<Model> playerObject)
{
	this->m_PlayerModel = playerObject;
}

void Player::rotate(float& movementSpeed, float& deltaTime)
{
	float velocity = movementSpeed * deltaTime;
	m_Rotation = glm::rotate(m_Rotation, glm::radians(20.0f*velocity), glm::vec3(0.0f, 1.0f, 0.0f));
}

glm::mat4 Player::getModelMatrix()
{
	return m_ModelMatrix;
}

std::vector<Model> Player::getPlayerobject()
{
	return m_PlayerModel;
}

glm::vec3 Player::getPlayerPosition()
{
	return m_Position;
}

glm::vec3 Player::getPlayerRight()
{
	return m_Right;
}

glm::vec3 Player::getPlayerFront()
{
	return m_Front;
}

void Player::move(GLFWwindow *window, float& deltaTime)
{
	processKeyboard(window, deltaTime);
	rotate(m_PlayerCurrentTurnSpeed, deltaTime);

	m_ModelMatrix = glm::translate(m_ModelMatrix, m_MoveVector);
}

void Player::getDown(float& deltaTime)
{
	m_PlayerVelocity = m_PlayerRunSpeed * deltaTime;
	m_MoveVector = glm::vec3(0);
	//std::cout << "This is Y : " << position.y << std::endl;
	if (m_Position.y > 15.0f) {
		m_MoveVector.y -= m_PlayerVelocity;
		m_Position += m_MoveVector;
		this->m_ModelMatrix = glm::translate(m_ModelMatrix, m_MoveVector);

	}
}