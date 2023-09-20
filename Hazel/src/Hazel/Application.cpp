#include "hzpch.h"
#include "Application.h"

//#include "Events/ApplicationEvent.h"

#include "Hazel/Log.h"

#include <GLFW/glfw3.h>

namespace Hazel {

#define BIND_EVENT_FN(x) std::bind(&Application::x, this, std::placeholders::_1)

	Application::Application()
	{
		m_Window = std::unique_ptr<Window>(Window::Create());
		// 等价于std::bind(&Application::OnEvent, this, std::placeholders::_1);
		// 这里相当于给using EventCallbackFn = std::function<void(Event&)>;绑定的函数是OnEvent
		m_Window->SetEventCallback(BIND_EVENT_FN(OnEvent));
		// 或者可以写成lambda表达式
		// m_Window->SetEventCallback([](Event& e) {HZ_CORE_TRACE("{0}", e); });
	}

	Application::~Application()
	{
	}

	void Application::OnEvent(Event& e)
	{
		// OnEvent会创建一个EventDispatcher对象，然后调用Dispatch函数
		EventDispatcher dispatcher(e);
		// 这个dispatcher会检测当前的事件是不是WindowCloseEvent，如果是的话就会调用OnWindowClose函数
		bool tmp = dispatcher.Dispatch<WindowCloseEvent>(BIND_EVENT_FN(OnWindowClose));

		
		HZ_CORE_TRACE("{0}", e);
	}

	void Application::Run()
	{
		while (m_Running)
		{
			glClearColor(1, 0, 1, 1);
			glClear(GL_COLOR_BUFFER_BIT);
			m_Window->OnUpdate();
		}
	}

	bool Application::OnWindowClose(WindowCloseEvent& e)
	{
		m_Running = false;
		return true;
	}

}