#include "stdafx.h"
#include "Player.h"

								////////////////////////////////////////////////////////////////
								// THE PLAYER CLASS											  //
								// This class binds an object to the camera and processes	  //
								// the move interactions									  //
								////////////////////////////////////////////////////////////////

extern Player player;

Player::Player(glm::vec3 startPosition)
{
	m_Position = startPosition;
	Update();
}

void Player::Update()
{
	m_ModelMatrix = glm::mat4(1.0f);
	m_ModelMatrix = glm::translate(m_ModelMatrix, m_Position);
	//m_ModelMatrix = glm::rotate(m_ModelMatrix, 90.0f, glm::vec3(0, 1, 0));
	//m_ModelMatrix = glm::scale(m_ModelMatrix, glm::vec3(0.5f, 0.5f, 0.5f));
}

void Player::setPlayerModel(std::vector<AnimModel> playerObject)
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

std::vector<AnimModel> Player::getPlayerobject()
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

	Update();
}