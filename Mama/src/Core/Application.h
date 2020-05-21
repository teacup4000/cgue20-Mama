#pragma once
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <glad/glad.h>

#include "INIReader.h"
#include "../Event/Events.h"
#include "../Render/Display/Player.h"

#define GLM_ENABLE_EXPERIMENTAL
#include <gtx/string_cast.hpp>

#include "../Render/Effects/Bloom.h"
#include "../Render/Effects/ShadowMap.h"


class Application
{
public:

	Application() 
	{
		ReadINIFile();
	}

	void Construct(Player* player, Camera*& camera) {
		m_Player = player;
		m_Camera = camera;
		event = Event(m_Camera, m_Width, m_Height);

		std::cout << "Sensitivity: " << m_Camera->getSensitivity() << std::endl;
		Run();
	}

	~Application() = default;
	void Run();
	void CreateGLFWWindow();


private: //MEMBERS
	GLFWwindow* m_Window;
	Player*	m_Player;
	Camera* m_Camera;

	float	m_Nearplane = 1.0f, m_Farplane = 100.0f;
	int		m_Width, m_Height;
	float	m_Brightness;
	bool	m_FullScreen;
	bool	m_NormalMap;
	bool	m_Shadow;

private: //FUNCTIONS
	Event event;

	void ReadINIFile() {
		INIReader iniReader("Assets/settings.ini");

		m_Width = iniReader.GetInteger("window", "width", 1600);
		m_Height = iniReader.GetInteger("window", "height", 900);

		m_Brightness = iniReader.GetFloat("shader", "brightness", 0.1f);
		m_NormalMap = iniReader.GetBoolean("shader", "normalMap", true);
		m_Shadow = iniReader.GetBoolean("shader", "shadow", true);
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
		//glEnable(GL_CULL_FACE); //Enable cull face if you don't want the program to render the back side of your objects 
		//glCullFace(GL_BACK);
		glEnable(GL_BLEND);;
		//glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); //with this no bump mapping/normal mapping/alpha mapping is possible but less memory usage!
		glEnable(GL_MULTISAMPLE);
	}

	void SetFrameRateIndependency(float& deltaTime, float& lastFrame)
	{
		float currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;
	}

	void SetInitialGLFWEvents()
	{
		glfwSetWindowUserPointer(m_Window, &event);

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

};