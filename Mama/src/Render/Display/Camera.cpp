#include "stdafx.h"
#include "Camera.h"

										////////////////////////////////////////////////////////////////
										// THE CAMERA CLASS											  //
										// This class processes the camera movement and interactions  //
										////////////////////////////////////////////////////////////////

Camera::Camera(Player* player, glm::vec3 currPosition)
	: m_Player(player), m_Position(currPosition)
{
	m_Distance = 3.0f;
	m_WorldUp = glm::vec3(0.0f, 1.0f, 0.0f);
	m_Yaw = 1.2f;
	m_Pitch = -31.7f;
	m_Front = glm::vec3(1.0f, 0.0f, -1.0f);
	m_MovementSpeed = 2.5f;
	m_MouseSensitivity = 0.1f;
	m_Zoom = 45.0f;

	updateCameraVectors();
}

//----------------------------------------------------------------------Camera funktions------------------------------------------------------------------------------------------
void Camera::processMouseMovement(float xoffset, float yoffset, GLboolean constrainPitch)
{
	
	xoffset *= getSensitivity();
	yoffset *= getSensitivity();

	m_Yaw += xoffset;
	m_Pitch += yoffset;

	/* Make sure that when pitch is out of bounds, screen doesn't get flipped */
	if (constrainPitch)
	{
		if (m_Pitch > 89.0f)
			m_Pitch = 89.0f;
		if (m_Pitch < -89.0f)
			m_Pitch = -89.0f;
	}

	if (!endScreen) {
		m_Position = m_Player->getPlayerPosition();
	}

	/* Update Front, Right and Up Vectors using the updated Euler angles */
	this->updateCameraVectors();
}

glm::mat4 Camera::getViewMatrix()
{
	if (!endScreen) {
		m_Position = m_Player->getPlayerPosition();

		glm::vec3 position = m_Position - m_Front * m_Distance;

		//---------------------------------------COLLISION CHECK---------------------------------------------------
		PxVec3 camPos(position.x, position.y, position.z);
		PxVec3 playerPos(m_Player->getPlayerPosition().x, m_Player->getPlayerPosition().y, m_Player->getPlayerPosition().z);
		float distance = m_physx->checkCamera(camPos, playerPos);

		if (distance <= 0.0f) {
			distance = 0.01f;
		}
		else if (distance > m_Distance) {
			distance = m_Distance;
		}
		//-----------------------------------------END CEHCK-------------------------------------------------------

		m_Position -= m_Front * distance;

		return glm::lookAt(m_Position, m_Position + m_Front, m_WorldUp);
	}
	else {

		m_Position = m_EndPosition;
		m_Position -= m_Front * m_Distance;

		return glm::lookAt(m_Position, m_Position + m_Front, m_WorldUp);
	}
}

void Camera::processMouseScroll(float yOffset)
{
	if (m_Zoom >= 1.0f && m_Zoom <= 45.0f)
		m_Zoom -= yOffset;
	if (m_Zoom <= 1.0f)
		m_Zoom = 1.0f;
	if (m_Zoom >= 45.0f)
		m_Zoom = 45.0f;
}

void Camera::updateCameraVectors()
{
	// Calculate the new Front vector
	glm::vec3 frontNew;
	frontNew.x = cos(glm::radians(m_Yaw)) * cos(glm::radians(m_Pitch));
	frontNew.y = sin(glm::radians(m_Pitch));
	frontNew.z = sin(glm::radians(m_Yaw)) * cos(glm::radians(m_Pitch));
	m_Front = glm::normalize(frontNew);

	// Also re-calculate the Right and Up vector
	m_Right = glm::normalize(glm::cross(m_Front, m_WorldUp));  // Normalize the vectors, because their length gets closer to 0 the more you look up or down which results in slower movement.
	m_Up = glm::normalize(glm::cross(m_Right, m_Front));
	m_Player->setRight(m_Right);
	m_Player->setFront(-m_Front);
	m_Player->setFrontY(0);
	m_Player->setFront(glm::normalize(m_Player->getPlayerFront()));
}

glm::mat4 Camera::getHealthBarMat(float remainingHealth) {

	glm::mat4 healthBarMat(1.0f);

	glm::vec3 up = glm::cross(m_Front, m_Right);
	healthBarMat = glm::translate(healthBarMat, m_Position + m_Front * 0.10001f - m_Right * (0.06f + 0.0112f * (1.0f - remainingHealth)) + up * 0.038f);
	
	
	float angle;

	glm::vec3 normal(m_Right.z, 0.0f, -m_Right.x);
	float dot = m_Front.x * normal.x + m_Front.z * normal.z;
	if (m_Front.y >= 0) {
		angle = acos(dot);
	}
	else {
		angle = -acos(dot);
	}
	healthBarMat = glm::rotate(healthBarMat, angle, glm::vec3(m_Right));

	angle = atan2(m_Right.x, m_Right.z);
	healthBarMat = glm::rotate(healthBarMat, angle, glm::vec3(0, 1, 0));

	healthBarMat = glm::scale(healthBarMat, glm::vec3(0.01f, 0.01f, 0.01f * remainingHealth));
	
	char buf[4096], *p = buf;
	sprintf(buf, "health: %f\n", remainingHealth);
	OutputDebugString(buf);

	return healthBarMat;
}

void Camera::loseScreen() {
	endScreen = true;
	m_EndPosition = glm::vec3(-9.0f, -10.0f, -10.0f);
	m_Front = glm::vec3(-1, 0, 0);
	m_Yaw = -181.5f;
	m_Pitch = 0;
	m_Zoom = 45.0f;
}

void Camera::winScreen() {
	endScreen = true;
	m_EndPosition = glm::vec3(-9.0f, -10.0f, -5.0f);
	m_Front = glm::vec3(-1, 0, 0);
	m_Yaw = -181.5f;
	m_Pitch = 0;
	m_Zoom = 45.0f;
}

void Camera::Reset() {
	m_Distance = 3.0f;
	m_WorldUp = glm::vec3(0.0f, 1.0f, 0.0f);
	m_Yaw = 1.2f;
	m_Pitch = -31.7f;
	m_Front = glm::vec3(1.0f, 0.0f, -1.0f);
	m_MovementSpeed = 2.5f;
	m_MouseSensitivity = 0.1f;
	m_Zoom = 45.0f;

	endScreen = false;

	updateCameraVectors();
}

