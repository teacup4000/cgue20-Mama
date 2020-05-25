#include "stdafx.h"
#include "Events.h"


//_____________________________________________________

void Event::SetFullScreen()
{
	if (m_Fullscreen)
		glfwSetWindowMonitor(m_Window, glfwGetPrimaryMonitor(), 0, 0, m_Width, m_Height, 0); /*Fullscreen always uses the Primary Monitor! */
	else
		glfwSetWindowMonitor(m_Window, NULL, 0, 0, m_Width, m_Height, 0);
}

void Event::SetRestart()
{
	m_Restart = false;
}

void Event::OnKeyPressed(int key, int code, int action, int modifers)
{
	/* Layers are dependend to the "doTheLayer" function in UI.cpp. If changing something in this file, make sure you change it there too! */
	if (key == GLFW_KEY_F1 && action == GLFW_PRESS)
	{
		m_Fullscreen = !m_Fullscreen;
		SetFullScreen();
	}

	if (key == GLFW_KEY_F2 && action == GLFW_PRESS)
	{
		m_Restart = true;
	}

	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(m_Window, true);
	}

	if (glfwGetKey(m_Window, GLFW_KEY_1) == GLFW_PRESS)
	{
		m_Player->m_ShowModel = true;
	}
}

void Event::ProcessPlayerMovements(float &deltatime)
{

}

/** process input from Mouse buttons */
void Event::OnButtonClicked(int button, int action, int mods)
{
	if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_PRESS)
	{
		m_Cursor = !m_Cursor;
		if (m_Cursor)
		{
			std::cout << "cursor true" << std::endl;
			glfwSetInputMode(m_Window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
		}
		else
		{
			std::cout << "cursor false" << std::endl;
			glfwSetInputMode(m_Window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
		}
	}
}

/** whenever the window size changes this function is called */
void Event::OnFramebufferSizeCallback(int width, int height)
{
	glViewport(0, 0, width, height);
}

/** whenever the mouse moves, this function is called */
void Event::OnCursorPos(double xPos, double yPos)
{
	if (m_FirstMouse)
	{
		m_LastX = xPos;
		m_LastY = yPos;
		m_FirstMouse = false;
	}
	float xOffset = xPos - m_LastX;
	float yOffset = m_LastY - yPos;

	m_LastX = xPos;
	m_LastY = yPos;

	m_Camera->processMouseMovement(xOffset, yOffset);
}

/** whenever the mouse scroll wheel scrolls, this function is called */
void Event::OnMouseScrolled(double xOffset, double yOffset)
{
	std::cout << yOffset << std::endl;
	m_Camera->processMouseScroll(yOffset);
}