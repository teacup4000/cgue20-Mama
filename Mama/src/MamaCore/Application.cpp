#include "mamapch.h"
#include "Application.h"

#include "Events/WindowEvent.h"

namespace Mama {

	Application::Application()
	{

	}

	Application::~Application()
	{

	}

	void Application::Run() {

		WindowResizeEvent e(1280, 720);
		MAMA_TRACE(e);

		while (true);
	}


}