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
	m_StartPosition = startPosition;
	Update();
}

void Player::Update()
{
	m_ModelMatrix = glm::mat4(1.0f);
	m_ModelMatrix = glm::translate(m_ModelMatrix, m_Position);

	
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
}

void Player::Reset() {
	m_ShowModel = false;
	m_Position = m_StartPosition;
	m_Controller->setPosition(PxExtendedVec3(m_StartPosition.x, m_StartPosition.y, m_StartPosition.z));
	positionSet = false;
	m_Right = glm::vec3(1, 0, 0);
	m_Front = glm::vec3(0, 0, 1);
	m_FrontOld = glm::vec3(0, 0, 0);
	lastJump = 0;
	lastMoveTime = -1.0f;
}

void Player::setPlayerModel(std::vector<AnimModel> playerObject)
{
	this->m_PlayerModel = playerObject;
}

void Player::move(GLFWwindow *window, float& deltaTime)
{
	processKeyboard(window, deltaTime);
	Update();
}

void Player::processKeyboard(GLFWwindow *window, float& deltaTime) {
	m_PlayerVelocity = m_PlayerRunSpeed * deltaTime;
	m_MoveVector = glm::vec3(0);
	if (!cameraControl) {
		m_MoveVector.y = m_MoveVector.y - GRAVITY * deltaTime * 250;
	}

	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) {
		m_MoveVector.z = -m_PlayerVelocity * m_Front.z;
		m_MoveVector.x = -m_PlayerVelocity * m_Front.x;
	}
	else if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) {
		m_MoveVector.z = m_PlayerVelocity * m_Front.z;
		m_MoveVector.x = m_PlayerVelocity * m_Front.x;
	}
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) {
		m_MoveVector.x = -m_PlayerVelocity * m_Right.x;
		m_MoveVector.z = -m_PlayerVelocity * m_Right.z;
	}
	else if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) {
		m_MoveVector.x = m_PlayerVelocity * m_Right.x;
		m_MoveVector.z = m_PlayerVelocity * m_Right.z;
	}

	if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
	{
		if (!cameraControl) {
			std::chrono::duration<float, std::milli> now = std::chrono::high_resolution_clock::now().time_since_epoch() / 1000;
			if (!jumped && groundContact && (lastJump == 0 || now.count() - lastJump > 1))
			{
				m_MoveVector.y = 0.025f * deltaTime * 250;
				jumped = true;
				std::chrono::duration<float, std::milli> last = std::chrono::high_resolution_clock::now().time_since_epoch();
				lastJump = last.count() / 1000;
			}
			else if (!peaked && jumped && m_Position.y - jumpHeight < 1 && m_Position.y - jumpHeight >= 0) {
				m_MoveVector.y = 0.025f * deltaTime * 250;
			}
			else if (!peaked && jumped && m_Position.y - jumpHeight >= 0.5) {
				peaked = true;
			}
		}
		else {
			m_MoveVector.y = m_PlayerVelocity;
		}
	} else if (glfwGetKey(window, GLFW_KEY_X) == GLFW_PRESS) {
		if (cameraControl) {
			m_MoveVector.y = -m_PlayerVelocity;
		}
	}

	else
	{
		if (jumped && groundContact) {
			jumped = false;
			peaked = false;
		}
		jumpHeight = m_Position.y;
	}

	if (!cameraControl) {
		//-----Physx chatacter controller movement------
		std::chrono::duration<float, std::milli> now = std::chrono::high_resolution_clock::now().time_since_epoch();
		float duration = lastMoveTime < 0 ? 0 : now.count() - lastMoveTime; // if first move then lastMove=-1 and no time passed, else calculate time passed
		lastMoveTime = now.count();


		physx::PxVec3 disp = physx::PxVec3(m_MoveVector.x, m_MoveVector.y, m_MoveVector.z);
		PxControllerCollisionFlags flags = m_Controller->move(disp, 0.01f, duration / 1000, NULL);

		int mask = 1 << 2;
		int maskedFlags = flags.operator uint8_t() & mask;
		int collisionDown = maskedFlags >> 2;

		if (collisionDown == 1) {
			groundContact = true;
		}
		else {
			groundContact = false;
		}

		m_Position.x = m_Controller->getFootPosition().x;
		m_Position.y = m_Controller->getFootPosition().y + 0.1;
		m_Position.z = m_Controller->getFootPosition().z;
	}
	/*char buf[4096], *p = buf;
	sprintf(buf, "right %f %f %f\n", getPlayerRight()[0], getPlayerRight()[1], getPlayerRight()[2]);
	OutputDebugString(buf);*/
}

glm::vec3 Player::moveCamera(GLFWwindow *window, float& deltaTime) {
	cameraControl = true;
	processKeyboard(window, deltaTime);
	cameraControl = false;
	return m_MoveVector;
}