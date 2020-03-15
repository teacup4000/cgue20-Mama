#pragma once
#include "Core.h"

#include "Events/Event.h"
#include "Events/WindowEvent.h"

#include "Layer/LayerStack.h"
#include "ImGui/ImguiLayer.h"

#include "Window/Window.h"
#include "Renderer/Shader.h"

namespace Mama {

	class Application
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
		ImguiLayer* m_ImguiLayer;
		bool m_Running = true;
		LayerStack m_LayerStack;
		unsigned int m_VertexArray, m_VertexBuffer, m_IndexBuffer;
		std::unique_ptr<Shader> m_Shader;

		static Application* s_Instance;

		//----------------FUNC--------------------
		bool OnWindowClose(WindowCloseEvent& e);
	};

	Application* CreateApplication();
}
