#pragma once

#ifdef MAMA_PLATFORM_WINDOWS

extern Mama::Application* Mama::CreateApplication();

int main(int argc, char** argv)
{
	Mama::Log::Init();
	Mama::Log::GetCoreLogger()->warn("initialized log");
	Mama::Log::GetClientLogger()->info("hello");

	auto app = Mama::CreateApplication();
	app->Run();
	delete app;
}

#endif