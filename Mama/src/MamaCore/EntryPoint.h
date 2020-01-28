#pragma once

#ifdef MAMA_PLATFORM_WINDOWS

extern Mama::Application* Mama::CreateApplication();

int main(int argc, char** argv)
{
	auto app = Mama::CreateApplication();
	app->Run();
	delete app;
}

#endif