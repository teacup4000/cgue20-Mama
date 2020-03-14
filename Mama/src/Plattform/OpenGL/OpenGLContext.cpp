#include "mamapch.h"
#include "OpenGLContext.h"

#include <GLFW/glfw3.h>
#include <glad/glad.h>
#include <GL/GL.h>

namespace Mama
{
	OpenGLContext::OpenGLContext(GLFWwindow* glfwWindow)
		:m_GlfwWindow(glfwWindow)
	{
		MAMA_CORE_ASSERT(glfwWindow, "Window is null");
	}

	void OpenGLContext::Init()
	{
		glfwMakeContextCurrent(m_GlfwWindow);
		int status = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
		MAMA_CORE_ASSERT(status, "Failed inizialize GLAD");
	}

	void OpenGLContext::SwapBuffers()
	{
		glfwSwapBuffers(m_GlfwWindow);
	}
}