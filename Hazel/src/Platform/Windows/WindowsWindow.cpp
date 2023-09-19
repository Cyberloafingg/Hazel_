#include "hzpch.h"
#include "WindowsWindow.h"


namespace Hazel {

	static bool s_GLFWInitialized = false;

	//����ʹ�õ��Ƿ���һ��Window*��ָ�룬������һ��WindowWindow*��ָ�룬�����ڿ�ƽ̨��ʱ��ֻ��Ҫ�޸����Create�����Ϳ�����
	Window* Window::Create(const WindowProps& props)
	{
		return new WindowsWindow(props);
	}

	// WindowsWindow���캯��
	WindowsWindow::WindowsWindow(const WindowProps& props)
	{
		Init(props);
	}

	WindowsWindow::~WindowsWindow()
	{
		Shutdown();
	}

	void WindowsWindow::Init(const WindowProps& props)
	{
		m_Data.Title = props.Title;
		m_Data.Width = props.Width;
		m_Data.Height = props.Height;

		HZ_CORE_INFO("Creating window {0} ({1}, {2})", props.Title, props.Width, props.Height);

		if (!s_GLFWInitialized)
		{
			// TODO: glfwTerminate on system shutdown
			int success = glfwInit();
			HZ_CORE_ASSERT(success, "Could not intialize GLFW!");

			s_GLFWInitialized = true;
		}

		// ��������
		m_Window = glfwCreateWindow((int)props.Width, (int)props.Height, m_Data.Title.c_str(), nullptr, nullptr);
		// ����ǰ�Ĵ�������Ϊ������
		glfwMakeContextCurrent(m_Window);
		// ���ô��ڵĻص�����
		// glfwSetWindowUserPointer���ڽ�һ���Զ���ָ�루ͨ����һ��ָ���û���������ݽṹ��ָ�룩������һ�����ڶ����ϡ����������Ŀ���������ܹ��ڴ��ڵ��¼��������з����Զ������ݣ��Ա��ڴ����¼�ʱ�ܹ����ʺͲ������Ӧ�ó������ݡ�
		glfwSetWindowUserPointer(m_Window, &m_Data);
		// ���ô�ֱͬ��
		SetVSync(true);
	}

	void WindowsWindow::Shutdown()
	{
		glfwDestroyWindow(m_Window);
	}

	void WindowsWindow::OnUpdate()
	{
		// glfwPollEvents���������û�д���ʲô�¼�������������롢����ƶ��ȣ������´���״̬�������ö�Ӧ�Ļص�����������ͨ���ص������ֶ����ã���������ֻ�Ǽ���¼��Ƿ������������������ǡ�
		glfwPollEvents();
		// glfwSwapBuffers�����ύ����ɫ���壬������һ�����б��������ƣ����ҽ�����Ϊ�����ʾ����Ļ�ϡ�
		glfwSwapBuffers(m_Window);
	}

	// SetVSync�����������ô�ֱͬ�����������Ϊtrue����ô���ǵ�Ӧ�ó��򽫻�����ʾ����ˢ���ʽ�����Ⱦ��������ǵ�Ӧ�ó�����Ⱦ����ʾ����ˢ������һ���ģ���ô�Ͳ������tearing�������
	void WindowsWindow::SetVSync(bool enabled)
	{
		if (enabled)
			// glfwSwapInterval�����������ô�ֱͬ��������Ϊ1ʱ����ֱͬ������������Ϊ0ʱ����ֱͬ���رա�
			glfwSwapInterval(1);
		else
			glfwSwapInterval(0);

		m_Data.VSync = enabled;
	}

	bool WindowsWindow::IsVSync() const
	{
		return m_Data.VSync;
	}

}