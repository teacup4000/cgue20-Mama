#include "stdafx.h"
#include "Camera.h"

Camera::Camera(Player* player, glm::vec3 currPosition)
	: m_Player(player), m_Position(currPosition)
{
	m_Distance = 3.0f;
	m_WorldUp = glm::vec3(0.0f, 1.0f, 0.0f);
	m_Yaw = -90.0f;
	m_Pitch = 0.0f;
	m_Front = glm::vec3(0.0f, 0.0f, -1.0f);
	m_MovementSpeed = 2.5f;
	m_MouseSensitivity = 0.1f;
	m_Zoom = 45.0f;

	updateCameraVectors();
}

//----------------------------------------------------------------------Camera funktions------------------------------------------------------------------------------------------
void Camera::processMouseMovement(float xoffset, float yoffset, GLboolean constrainPitch)
{
	//this->position = this->player->position;
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

	m_Position = m_Player->position;

	/* Update Front, Right and Up Vectors using the updated Euler angles */
	this->updateCameraVectors();

	m_Position -= m_Front * m_Distance;
}

glm::mat4 Camera::getViewMatrix()
{
	m_Position = m_Player->position;
	m_Position -= m_Front * m_Distance;
	return glm::lookAt(m_Position, m_Position + m_Front, m_WorldUp);
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
	m_Player->right = m_Right;
	m_Player->front = -m_Front;
	m_Player->front.y = 0;
	m_Player->front = glm::normalize(m_Player->front);
}

