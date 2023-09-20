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
		// �ȼ���std::bind(&Application::OnEvent, this, std::placeholders::_1);
		// �����൱�ڸ�using EventCallbackFn = std::function<void(Event&)>;�󶨵ĺ�����OnEvent
		m_Window->SetEventCallback(BIND_EVENT_FN(OnEvent));
		// ���߿���д��lambda���ʽ
		// m_Window->SetEventCallback([](Event& e) {HZ_CORE_TRACE("{0}", e); });
	}

	Application::~Application()
	{
	}

	void Application::OnEvent(Event& e)
	{
		// OnEvent�ᴴ��һ��EventDispatcher����Ȼ�����Dispatch����
		EventDispatcher dispatcher(e);
		// ���dispatcher���⵱ǰ���¼��ǲ���WindowCloseEvent������ǵĻ��ͻ����OnWindowClose����
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