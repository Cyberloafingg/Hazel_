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

	//这里使用的是返回一个Window*的指针，而不是一个WindowWindow*的指针，这样在跨平台的时候，只需要修改这个Create函数就可以了
	Window* Window::Create(const WindowProps& props)
	{
		return new WindowsWindow(props);
	}

	// WindowsWindow构造函数
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

		// 创建窗口
		m_Window = glfwCreateWindow((int)props.Width, (int)props.Height, m_Data.Title.c_str(), nullptr, nullptr);
		// 将当前的窗口设置为主窗口
		glfwMakeContextCurrent(m_Window);

		int status = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
		HZ_CORE_ASSERT(status, "Failed to initialize Glad!");


		// 设置窗口的回调函数
		// glfwSetWindowUserPointer用于将一个自定义指针（通常是一个指向用户定义的数据结构的指针）关联到一个窗口对象上。这个函数的目的是让你能够在窗口的事件处理函数中访问自定义数据，以便在处理事件时能够访问和操作你的应用程序数据。
		glfwSetWindowUserPointer(m_Window, &m_Data);
		// 设置垂直同步
		SetVSync(true);

		// 设置窗口的回调函数：回调函数是在GLFW中通过事件循环触发的，当事件发生时，GLFW会检测这些事件并自动调用相关的回调函数
		glfwSetWindowSizeCallback(m_Window, [](GLFWwindow* window, int width, int height)
		{
			// glfwGetWindowUserPointer函数用于返回与窗口window关联的指针。
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
			/*
			 * glfwGetWindowUserPointer(window) 返回一个 void* 指针，指向与窗口关联的用户数据。
			 * (WindowData*) 这一部分是将 void* 指针强制类型转换为 WindowData* 类型的指针。这个操作告诉编译器将 void* 指针视为指向 WindowData 结构体类型的指针。
			 * * 【(WindowData*)glfwGetWindowUserPointer(window)】 是将 WindowData* 指针解引用，以便访问与窗口关联的实际 WindowData 数据。现在，data 变量包含了指向 WindowData 结构体的指针，你可以使用它来访问窗口属性和事件回调函数。
			 */
			// 设置窗口的宽度和高度
			data.Width = width;
			data.Height = height;

			// 创建一个窗口调整大小的事件
			WindowResizeEvent event(width, height);
			// 调用窗口的事件回调函数
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
		// glfwPollEvents函数检查有没有触发什么事件（比如键盘输入、鼠标移动等）、更新窗口状态，并调用对应的回调函数（可以通过回调方法手动设置）。它仅仅只是检查事件是否发生，并立即处理它们。
		glfwPollEvents();
		// glfwSwapBuffers函数会交换颜色缓冲，它在这一迭代中被用来绘制，并且将会作为输出显示在屏幕上。
		glfwSwapBuffers(m_Window);
	}

	// SetVSync函数用于设置垂直同步，如果设置为true，那么我们的应用程序将会以显示器的刷新率进行渲染。如果我们的应用程序渲染和显示器的刷新率是一样的，那么就不会出现tearing的情况。
	void WindowsWindow::SetVSync(bool enabled)
	{
		if (enabled)
			// glfwSwapInterval函数用于设置垂直同步，参数为1时，垂直同步开启，参数为0时，垂直同步关闭。
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