#pragma once
#include "Core.h"
#include "Events/Event.h"
#include "Window.h"

namespace Hazel
{
	class HAZEL_API Application
	{
	public:
		Application();
		virtual ~Application();
		void Run();
	private:
		std::shared_ptr<Window> m_Window;
		bool m_Running = true;
	};

	// To be defined in CLIENT,在客户端定义
	Application* CreateApplication();
}


