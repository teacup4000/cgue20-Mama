#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include "../Render/Display/Camera.h"

class Event
{
public:
	Event() = default;
	Event(Camera* camera, int width, int height)
		: m_Camera(camera), m_Width(width), m_Height(height)
	{
		m_Fullscreen = false;
		m_Cursor = false;
		m_LastX = (float)(m_Width / 2.0f);
		m_LastY = (float)(m_Height / 2.0f);
	}

	void OnFramebufferSizeCallback(int width, int heigth);
	void OnCursorPos(double posX, double posY);
	void OnMouseScrolled(double xOffset, double yOffset);

	void OnKeyPressed(int key, int button, int action, int mods);

	void OnButtonClicked(int button, int action, int mods);
	void SetNativeWindow(GLFWwindow* window) { m_Window = window; }

	void SetFullScreen();

private:
	GLFWwindow* m_Window;
	Camera* m_Camera;

	bool m_Fullscreen, m_FirstMouse, m_Cursor;
	float m_LastX, m_LastY;
	int m_Width, m_Height;
};


