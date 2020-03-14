#pragma once

#include "Renderer/RenderingContext.h"

struct GLFWwindow;

namespace Mama
{
	class OpenGLContext : public RenderingContext
	{
	public:
		OpenGLContext(GLFWwindow* glfwWindow);

		virtual void Init() override;
		virtual void SwapBuffers() override;

	private:
		GLFWwindow* m_GlfwWindow;
	};
}