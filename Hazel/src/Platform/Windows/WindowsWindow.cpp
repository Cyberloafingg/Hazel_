#include "hzpch.h"
#include "WindowsWindow.h"


namespace Hazel {

	static bool s_GLFWInitialized = false;

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

			s_GLFWInitialized = true;
		}

		// 创建窗口
		m_Window = glfwCreateWindow((int)props.Width, (int)props.Height, m_Data.Title.c_str(), nullptr, nullptr);
		// 将当前的窗口设置为主窗口
		glfwMakeContextCurrent(m_Window);
		// 设置窗口的回调函数
		// glfwSetWindowUserPointer用于将一个自定义指针（通常是一个指向用户定义的数据结构的指针）关联到一个窗口对象上。这个函数的目的是让你能够在窗口的事件处理函数中访问自定义数据，以便在处理事件时能够访问和操作你的应用程序数据。
		glfwSetWindowUserPointer(m_Window, &m_Data);
		// 设置垂直同步
		SetVSync(true);
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