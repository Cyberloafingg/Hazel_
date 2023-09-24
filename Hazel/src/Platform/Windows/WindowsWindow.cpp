#include "hzpch.h"
#include "WindowsWindow.h"

#include "Hazel/Events/ApplicationEvent.h"
#include "Hazel/Events/MouseEvent.h"
#include "Hazel/Events/KeyEvent.h"

#include <glad/glad.h>


namespace Hazel {

	static bool s_GLFWInitialized = false;

	static void GLFWErrorCallback(int error, const char* description)
	{
		HZ_CORE_ERROR("GLFW Error ({0}): {1}", error, description);
	}

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

			glfwSetErrorCallback(GLFWErrorCallback);
			s_GLFWInitialized = true;
		}

		// ��������
		m_Window = glfwCreateWindow((int)props.Width, (int)props.Height, m_Data.Title.c_str(), nullptr, nullptr);
		// ����ǰ�Ĵ�������Ϊ������
		glfwMakeContextCurrent(m_Window);

		int status = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
		HZ_CORE_ASSERT(status, "Failed to initialize Glad!");


		// ���ô��ڵĻص�����
		// glfwSetWindowUserPointer���ڽ�һ���Զ���ָ�루ͨ����һ��ָ���û���������ݽṹ��ָ�룩������һ�����ڶ����ϡ����������Ŀ���������ܹ��ڴ��ڵ��¼��������з����Զ������ݣ��Ա��ڴ����¼�ʱ�ܹ����ʺͲ������Ӧ�ó������ݡ�
		glfwSetWindowUserPointer(m_Window, &m_Data);
		// ���ô�ֱͬ��
		SetVSync(true);

		// ���ô��ڵĻص��������ص���������GLFW��ͨ���¼�ѭ�������ģ����¼�����ʱ��GLFW������Щ�¼����Զ�������صĻص�����
		glfwSetWindowSizeCallback(m_Window, [](GLFWwindow* window, int width, int height)
		{
			// glfwGetWindowUserPointer�������ڷ����봰��window������ָ�롣
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
			/*
			 * glfwGetWindowUserPointer(window) ����һ�� void* ָ�룬ָ���봰�ڹ������û����ݡ�
			 * (WindowData*) ��һ�����ǽ� void* ָ��ǿ������ת��Ϊ WindowData* ���͵�ָ�롣����������߱������� void* ָ����Ϊָ�� WindowData �ṹ�����͵�ָ�롣
			 * * ��(WindowData*)glfwGetWindowUserPointer(window)�� �ǽ� WindowData* ָ������ã��Ա�����봰�ڹ�����ʵ�� WindowData ���ݡ����ڣ�data ����������ָ�� WindowData �ṹ���ָ�룬�����ʹ���������ʴ������Ժ��¼��ص�������
			 */
			// ���ô��ڵĿ�Ⱥ͸߶�
			data.Width = width;
			data.Height = height;

			// ����һ�����ڵ�����С���¼�
			WindowResizeEvent event(width, height);
			// ���ô��ڵ��¼��ص�����
			data.EventCallback(event);
		});

		glfwSetWindowCloseCallback(m_Window, [](GLFWwindow* window)
		{
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
			WindowCloseEvent event;
			data.EventCallback(event);
		});

		glfwSetKeyCallback(m_Window, [](GLFWwindow* window, int key, int scancode, int action, int mods)
		{
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
			switch (action)
			{
				case GLFW_PRESS:
				{
					KeyPressedEvent event(key, 0);
					data.EventCallback(event);
					break;
				}
				case GLFW_RELEASE:
				{
					KeyReleasedEvent event(key);
					data.EventCallback(event);
					break;
				}
				case GLFW_REPEAT:
				{
					KeyPressedEvent event(key, 1);
					data.EventCallback(event);
					break;
				}
			}
		});

		glfwSetCharCallback(m_Window, [](GLFWwindow* window, unsigned int keycode)
		{
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

			KeyTypedEvent event(keycode);
			data.EventCallback(event);
		});


		glfwSetMouseButtonCallback(m_Window, [](GLFWwindow* window, int button, int action, int mods)
		{
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

			switch (action)
			{
				case GLFW_PRESS:
				{
					MouseButtonPressedEvent event(button);
					data.EventCallback(event);
					break;
				}
				case GLFW_RELEASE:
				{
					MouseButtonReleasedEvent event(button);
					data.EventCallback(event);
					break;
				}
			}
		});

		glfwSetScrollCallback(m_Window, [](GLFWwindow* window, double xOffset, double yOffset)
		{
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

			MouseScrolledEvent event((float)xOffset, (float)yOffset);
			data.EventCallback(event);
		});

		glfwSetCursorPosCallback(m_Window, [](GLFWwindow* window, double xPos, double yPos)
		{
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

			MouseMovedEvent event((float)xPos, (float)yPos);
			data.EventCallback(event);
		});
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