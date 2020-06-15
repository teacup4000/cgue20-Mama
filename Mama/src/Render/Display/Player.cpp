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
	
	//atan(det, dot)
	//dot = x * 0 + z * 1
	//det = x * 1 - z * 0
	float angle = atan2(m_Front.x, m_Front.z);
	m_ModelMatrix = glm::rotate(m_ModelMatrix, angle, glm::vec3(0, 1, 0));
	if (m_Controller) {
		if (!positionSet) {
			float zNorm = m_Front.z / sqrt(pow(m_Front.z, 2) + pow(m_Front.x, 2));
			float xNorm = m_Front.x / sqrt(pow(m_Front.z, 2) + pow(m_Front.x, 2));
			PxVec3 normal = PxVec3(-zNorm, 0.0f, xNorm);
			PxRigidDynamic *cActor = m_Controller->getActor();
			PxShape *cShapes[1];
			cActor->getShapes(cShapes, 1, 0);
			PxShape *cShape = cShapes[0];
			cShape->setLocalPose(PxTransform(PxQuat(1.5708, normal)));
			positionSet = true;
		}
		else {
			float dot = m_Front.x * m_FrontOld.x + m_Front.z * m_FrontOld.z;
			float det = m_Front.x* m_FrontOld.z - m_Front.z * m_FrontOld.x;
			angle = atan2(det, dot);
			PxRigidDynamic *cActor = m_Controller->getActor();
			PxShape *cShapes[1];
			cActor->getShapes(cShapes, 1, 0);
			PxShape *cShape = cShapes[0];
			cShape->setLocalPose(PxTransform(PxQuat(angle, PxVec3(1, 0, 0))).transform(cShape->getLocalPose()));
		}
	}
	m_FrontOld = m_Front;
	//m_ModelMatrix = glm::rotate(m_ModelMatrix, m_PlayerRotation, glm::vec3(0, 1, 0));
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