#include <Mama.h>

class ExampleLayer : public Mama::Layer
{
	public:
		ExampleLayer()
			: Layer("Example")
		{
		}

		void OnUpdate() override
		{
		}

		void OnEvent(Mama::Event& event) override
		{
			MAMA_TRACE("{0}", event);
		}
};

class Sandbox : public Mama::Application
{
public:
	Sandbox()
	{
		PushLayer(new ExampleLayer());
		PushLayer(new Mama::ImguiLayer());
	}

	~Sandbox()
	{

	}
};

Mama::Application* Mama::CreateApplication() {
	return new Sandbox();
}