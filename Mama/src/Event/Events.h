#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "Render/Display/Camera.h"
#include "Render/Effects/FrustumCulling.h"

class Event
{
public:
	bool m_Restart;

	Event() = default;
	Event(Camera* camera, int width, int height, bool fullscreen)
		: m_Camera(camera), m_Width(width), m_Height(height), m_Fullscreen(fullscreen)
	{
		m_Frustum = false;
		m_Cursor = false;
		m_LastX = (float)(m_Width / 2.0f);
		m_LastY = (float)(m_Height / 2.0f);
	}

	void OnFramebufferSizeCallback(int width, int heigth);
	void OnCursorPos(double posX, double posY);
	void OnMouseScrolled(double xOffset, double yOffset);

	void OnKeyPressed(int key, int button, int action, int mods);
	void ProcessPlayerMovements(float& deltatime);

	void OnButtonClicked(int button, int action, int mods);
	void SetNativeWindow(GLFWwindow* window) { m_Window = window; }
	void setNativePlayer(Player* player) { m_Player = player; }
	void SetRestart();

	void SetFullScreen();
	
	bool GetFrustum() { return m_Frustum; }

private:
	GLFWwindow* m_Window;
	Camera* m_Camera;
	Player* m_Player;

	bool m_Fullscreen, m_FirstMouse, m_Cursor, m_Frustum;
	float m_LastX, m_LastY;
	int m_Width, m_Height;
};


