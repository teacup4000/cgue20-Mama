#include <Mama.h>
#include "ImGui/imgui.h"

class UILayer : public Mama::Layer
{
	public:
		UILayer()
			: Layer("ImGuiLayer")
		{
		}

		void OnUpdate() override
		{
		
		}

		virtual void OnImGuiRender() override
		{
			ImGui::ShowDemoWindow();
			ImGui::Begin("Test");
			ImGui::Text("Hello World");
			ImGui::End();
		}

		void OnEvent(Mama::Event& event) override
		{
			if (event.GetEventType() == Mama::EventType::KeyPressed)
			{
				Mama::KeyPressedEvent& e = (Mama::KeyPressedEvent&)event;
				MAMA_TRACE("{0}", 0);
				MAMA_TRACE("{1}", 1);
			}
		}
};

class Sandbox : public Mama::Application
{
public:
	Sandbox()
	{
		PushLayer(new UILayer());
	}

	~Sandbox()
	{

	}
};

Mama::Application* Mama::CreateApplication() 
{
	return new Sandbox();
}