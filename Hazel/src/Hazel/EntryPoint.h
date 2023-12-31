#pragma once

#ifdef HZ_PLATFORM_WINDOWS

extern Hazel::Application* Hazel::CreateApplication();

int main(int argc, char** argv)
{
	Hazel::Log::Init();
	HZ_CORE_WARN("-------------Initialized------------");

	const auto app = Hazel::CreateApplication();
	app->Run();
	delete app;
	std::vector<int> a = {1,2,3};
	a.emplace(a.begin(), 3);

}

#else
	#error Hazel only supports Windows!
#endif