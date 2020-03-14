#pragma once
//the entry point is how we enter the MAMA_API application from outside.

#ifdef MAMA_PLATFORM_WINDOWS

extern Mama::Application* Mama::CreateApplication();

int main(int argc, char** argv)
{
	Mama::Log::Init();
	MAMA_CORE_WARN("inizialized Logger");

	auto app = Mama::CreateApplication();
	app->Run();
	delete app;
}

#endif