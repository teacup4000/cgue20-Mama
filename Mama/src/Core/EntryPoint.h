#pragma once

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