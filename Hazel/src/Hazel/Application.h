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
		// �����Ĵ���
		std::shared_ptr<Window> m_Window;
		// ��ջ
		LayerStack m_LayerStack;
		// APP��ǰ�Ƿ�����
		bool m_Running = true;

	private:
		bool OnWindowClose(WindowCloseEvent& e);
	};

	// To be defined in CLIENT,�ڿͻ��˶���
	Application* CreateApplication();
}


