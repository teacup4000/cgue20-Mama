#include "Application.h"

#include "Events/WindowEvent.h"
#include "MamaCore/Log.h"

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