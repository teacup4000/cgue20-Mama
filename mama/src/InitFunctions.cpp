#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "INIReader.h"
#include <iostream>

//-------globals-----------
extern GLFWwindow window;
//-------function declarations------
void framebuffer_size_callback(GLFWwindow* window, int width, int heigth);
void mouse_callback(GLFWwindow* window, double xPos, double yPos);
void scroll_callback(GLFWwindow* window, double xOffset, double yOffest);
//--------------------------
/** Initialize the GLFW lib */
void initGlfw()
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

/** Initialize the GLEW lib and load OpenGL function pointers*/
void initGlad()
{
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
	}
}

/** GL enable functions */
void enableGL()
{
	glEnable(GL_DEPTH_TEST);
	//glEnable(GL_CULL_FACE); //Enable cull face if you don't want the program to render the back side of your objects 
	//glCullFace(GL_BACK);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); //with this no bump mapping/normal mapping/alpha mapping is possible but less memory usage!
	glEnable(GL_MULTISAMPLE);
}

/** compute frame rate independency */
void computeTimeLogic(float& deltaTime, float& lastFrame)
{
	float currentFrame = glfwGetTime();
	deltaTime = currentFrame - lastFrame;
	lastFrame = currentFrame;
}

/** set mouse options */
void initMouse(GLFWwindow* window)
{
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetScrollCallback(window, scroll_callback);
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
}

/** read the Init file in the folder Assets */
void initReader(GLint& width, GLint& height, float& brightness)
{
	/* Read the settings */
	INIReader iniReader("Assets/settings.ini");

	/* edit settings in settings.ini not here! */
	width = iniReader.GetInteger("window", "width", 1600);
	height = iniReader.GetInteger("window", "height", 900);
	brightness = iniReader.GetFloat("shader", "brightness", 0.1f);
}
