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
	void SetNativePlayer(Player* player) { m_Player = player; }
	void SetNativeGame(Game* game) { m_Game = game; }
	void SetRestart();
	float getVolume() { return m_Volume; }
	bool isMuted() { return m_Muted; }
	bool isRestart() { return m_Restart; }
	bool isWireFrame() { return m_Wireframe; }
	bool isCameraControl() { return m_CameraControl; }
	bool isRenderAll() { return m_RenderAll; }
	bool isVsync() { return m_Vsync; }
	bool isCube() { return m_Cube; }

	void SetFullScreen();
	
	bool isFrustum() { return m_Frustum; }

private:
	GLFWwindow* m_Window;
	Camera* m_Camera;
	Player* m_Player;
	Game* m_Game;

	bool m_Fullscreen, m_FirstMouse, m_Cursor, m_Frustum;
	bool m_Wireframe = false;
 	bool m_CameraControl = false;
	bool m_RenderAll = false;
	bool m_Muted = false;
	bool m_Vsync = false;
	bool m_Cube = false;

	float m_Volume = 0.1;

	float m_LastX, m_LastY;
	int m_Width, m_Height;
};


