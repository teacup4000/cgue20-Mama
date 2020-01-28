#include <Mama.h>


class Sandbox : public Mama::Application
{
public:
	Sandbox()
	{

	}

	~Sandbox()
	{

	}
};

Mama::Application* Mama::CreateApplication() {
	return new Sandbox();
}