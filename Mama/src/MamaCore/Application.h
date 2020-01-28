#pragma once
#include "Core.h"

namespace Mama {

	class MAMA_API Application
	{
	public:
		Application();
		virtual ~Application();

		void Run();
	
	};
	Application* CreateApplication();
}
