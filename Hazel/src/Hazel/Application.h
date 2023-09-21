#pragma once
#include "Core.h"
#include "Window.h"

#include "Hazel/LayerStack.h"
#include "Hazel/Events/Event.h"
#include "Hazel/Events/ApplicationEvent.h"

namespace Hazel
{
	class HAZEL_API Application
	{
	public:
		Application();
		virtual ~Application();

		void Run();

		void OnEvent(Event& e);

		void PushLayer(Layer* layer);
		void PushOverlay(Layer* layer);
	private:
		// 单独的窗口
		std::shared_ptr<Window> m_Window;
		// 层栈
		LayerStack m_LayerStack;
		// APP当前是否运行
		bool m_Running = true;

	private:
		bool OnWindowClose(WindowCloseEvent& e);
	};

	// To be defined in CLIENT,在客户端定义
	Application* CreateApplication();
}


