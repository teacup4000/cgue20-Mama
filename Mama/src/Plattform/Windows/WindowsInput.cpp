#include "mamapch.h"
#include "WindowsInput.h"

#include "Core/Application.h"

#include <GLFW/glfw3.h>

namespace Mama
{
	Input* Input::s_Instance = new WindowsInput();

	bool WindowsInput::IsKeyPressedImpl(int keycode)
	{
		auto window = static_cast<GLFWwindow*>(Application::Get().GetWindow().GetNativeWindow());

		auto state = glfwGetKey( window, keycode );
		return state == GLFW_PRESS || state == GLFW_REPEAT;
	}

	bool WindowsInput::IsMouseButtonPressedImpl(int button)
	{
		auto window = static_cast<GLFWwindow*>(Application::Get().GetWindow().GetNativeWindow());

		auto state = glfwGetMouseButton(window, button);
		return state == GLFW_PRESS;
	}

	std::pair<float, float> WindowsInput::GetMousePosImpl()
	{
		auto window = static_cast<GLFWwindow*>(Application::Get().GetWindow().GetNativeWindow());
		double xPos, yPos;
		glfwGetCursorPos(window, &xPos, &yPos);

		return { (float)xPos, (float)yPos };
	}

	float WindowsInput::GetXMouseImpl()
	{
		auto[x, y] = GetMousePosImpl();
		return x;
	}

	float WindowsInput::GetYMouseImpl()
	{
		auto[x, y] = GetMousePosImpl();
		return y;
	}
}