#pragma once
#include "Core.h"

#include "Events/Event.h"
#include "Events/WindowEvent.h"

#include "Layer/LayerStack.h"

#include "Window/Window.h"

namespace Mama {

	class MAMA_API Application
	{
	public:
		Application();
		virtual ~Application();

		void Run();

		void OnEvent(Event& e);
		void PushLayer(Layer* layer);
		void PushOverlay(Layer* layer);
	
		static inline Application& Get() { return *s_Instance; }
		inline Window& GetWindow() { return *m_Window; }

	private:
		//----------------VAR---------------------
		std::unique_ptr<Window> m_Window;
		bool m_Running = true;
		LayerStack m_LayerStack;

		static Application* s_Instance;

		//----------------FUNC--------------------
		bool OnWindowClose(WindowCloseEvent& e);
	};

	Application* CreateApplication();
}
