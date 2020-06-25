#pragma once
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <glad/glad.h>

#include "Game.h"

#include "INIReader.h"
#include "Event/Events.h"

#include "PhysX/PhysX.h"

#define GLM_ENABLE_EXPERIMENTAL
#include <gtx/string_cast.hpp>

#include "Render/Effects/Bloom.h"
#include "Render/Effects/ShadowMap.h"
#include "Render/Renderer.h"
#include "Render/Display/Player.h"
#include "Render/Effects/Hud.h"

#include "Sound/IrrKlang.h"


class Application
{
public:

	Application() 
	{
		ReadINIFile();
	}

	void Init(Player* player, Camera*& camera) {
		m_Player = player;
		m_Camera = camera;
		m_DeltaTime = 0.0f;
		m_LastFrame = 0.0f;
		m_Nearplane = 0.0f;
		m_Farplane = 100.0f;
		m_Event = Event(m_Camera, m_Width, m_Height, m_Fullscreen);

		Run();
	}

	~Application() = default;
	void Run();
	glm::mat4 getOrientationFromPos(PxTransform pos);
	void CreateGLFWWindow();


private: //MEMBERS
	GLFWwindow* m_Window;
	Player*	m_Player;
	Camera* m_Camera;
	Physx* m_PhysX = new Physx();
	Event m_Event;

	float	m_Nearplane, m_Farplane;
	float	m_DeltaTime, m_LastFrame;
	int		m_Width, m_Height;
	bool	m_Fullscreen;
	bool	m_NormalMap;
	bool	m_Shadow;
	uint32_t m_Refresh;
	glm::vec3	m_Brightness;

	std::vector<PxRigidDynamic*> m_DynamicObjects;

private: //FUNCTIONS

	void ReadINIFile() {
		INIReader iniReader("Assets/settings.ini");

		m_Width = iniReader.GetInteger("window", "width", 1600);
		m_Height = iniReader.GetInteger("window", "height", 900);
		m_Fullscreen = iniReader.GetBoolean("window", "fullscreen", false);

		float tempBrightness = iniReader.GetFloat("shader", "brightness", 0.1f);
		m_Brightness = glm::vec3(tempBrightness, tempBrightness, tempBrightness);
		m_NormalMap = iniReader.GetBoolean("shader", "normalMap", true);
		m_Shadow = iniReader.GetBoolean("shader", "shadow", true);

		m_Refresh = iniReader.GetInteger("GLFW", "refresh_rate", 0);
	}

	void InitGLFW()
	{
		GLint glfwInitResult = glfwInit();

		//Choosing properties and version
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
		glfwWindowHint(GLFW_RESIZABLE, GL_TRUE);
		glfwWindowHint(GLFW_SAMPLES, 4);
		glfwWindowHint(GLFW_REFRESH_RATE, m_Refresh);
		if (!glfwInitResult)
		{
			std::cout << "ERROR: GLFW not Initialized!" << std::endl;
			exit(EXIT_FAILURE);
		}
	}

	void InitGLAD()
	{
		if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
		{
			std::cout << "Failed to initialize GLAD" << std::endl;
			exit(EXIT_FAILURE);
		}
	}

	void EnableGL()
	{
		glEnable(GL_DEPTH_TEST);
		glEnable(GL_BLEND);;
		glEnable(GL_MULTISAMPLE);
	}

	void SetFrameRateIndependency()
	{
		float currentFrame = glfwGetTime();
		m_DeltaTime = currentFrame - m_LastFrame;
		m_LastFrame = currentFrame;
	}

	void SetInitialGLFWEvents()
	{
		glfwSetWindowUserPointer(m_Window, &m_Event);

		glfwSetInputMode(m_Window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

		glfwSetFramebufferSizeCallback(m_Window, [](GLFWwindow* window, int width, int height)
		{
			Event* event = static_cast<Event*>(glfwGetWindowUserPointer(window));
			event->OnFramebufferSizeCallback(width, height);
		});

		glfwSetCursorPosCallback(m_Window, [](GLFWwindow* window, double posX, double posY)
		{
			Event* event = static_cast<Event*>(glfwGetWindowUserPointer(window));
			event->OnCursorPos(posX, posY);
		});

		glfwSetScrollCallback(m_Window, [](GLFWwindow* window, double offsetX, double offsetY)
		{
			Event* event = static_cast<Event*>(glfwGetWindowUserPointer(window));
			event->OnMouseScrolled(offsetX, offsetY);
		});
	}

	void SetGLFWEvents()
	{
		m_Event.SetNativeWindow(m_Window);
		m_Event.SetNativePlayer(m_Player);
		glfwSetKeyCallback(m_Window, [](GLFWwindow* window, int key, int code, int action, int mods)
		{
		Event* event = static_cast<Event*>(glfwGetWindowUserPointer(window));
		event->OnKeyPressed(key,code,action,mods);
		});

		glfwSetMouseButtonCallback(m_Window, [](GLFWwindow* window, int button, int action, int mods)
		{		
			Event* event = static_cast<Event*>(glfwGetWindowUserPointer(window));
			event->SetNativeWindow(window);
			event->OnButtonClicked(button, action, mods);
		});

		glfwSetScrollCallback(m_Window, [](GLFWwindow* window, double offsetX, double offsetY)
		{
			Event* event = static_cast<Event*>(glfwGetWindowUserPointer(window));
			event->OnMouseScrolled(offsetX, offsetY);
		});
	}

	float getRad(float degree)
	{
		return degree * (glm::pi<float>() / 180);
	}

};