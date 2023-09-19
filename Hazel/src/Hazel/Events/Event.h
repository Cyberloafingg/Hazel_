#pragma once

#include "Hazel/Core.h"
#include "hzpch.h"
#include <string>
#include <functional>


namespace Hazel
{
	// Events in Hazel are currently blocking, meaning when an event occurs it
	// immediately gets dispatched and must be dealt with right then an there.
	// For the future, a better strategy might be to buffer events in an event
	// bus and process them during the "event" part of the update stage.
	// Hazel中的事件当前是阻塞的，这意味着当事件发生时，它会立即被分派并且必须立即处理。
	// 为了将来，一个更好的策略可能是在事件总线中缓冲事件，并在更新阶段的“事件”部分处理它们。


	// 事件类型
	enum class EventType
	{
		None = 0,
		WindowClose, WindowResize, WindowFocus, WindowLostFocus, WindowMoved,
		AppTick, AppUpdate, AppRender,
		KeyPressed, KeyReleased,
		MouseButtonPressed, MouseButtonReleased, MouseMoved, MouseScrolled
	};

	// 事件类型的分类
	/*
	 * 掩码技巧基于位运算，主要用于将不同的标志或状态组合在一起，以便在一个整数中表示多个状态或选项，节省内存和提高效率。
	   这个宏接受一个整数参数 x，并将1左移 x 位。例如，BIT(0) 会生成1，BIT(1) 会生成2，BIT(2) 会生成4，以此类推。这样的位掩码宏允许你快速生成具有单独位设置的整数值。
	   这个技巧的主要优势在于，你可以使用位运算来检查一个整数是否包含特定的标志，以及将标志添加到或从整数中移除。例如，你可以使用按位与运算 & 来检查一个事件是否属于某个特定类别：
	   比如：if (event.GetCategoryFlags() & EventCategoryMouse) { ... } 即可判断是否是是鼠标事件
	   或者：可以使用按位或运算 | 来将一个事件分配给多个类别：eventCategory |= EventCategoryKeyboard; // 将事件分配给键盘类别
	   
	 */
	enum EventCategory
	{
		None = 0,
		EventCategoryApplication = BIT(0),
		EventCategoryInput = BIT(1),
		EventCategoryKeyboard = BIT(2),
		EventCategoryMouse = BIT(3),
		EventCategoryMouseButton = BIT(4)
	};

// 下面的宏定义了三个函数，用于在运行时获取事件的类型、名称和类别。
#define EVENT_CLASS_TYPE(type) static EventType GetStaticType() { return EventType::##type; }\
								virtual EventType GetEventType() const override { return GetStaticType(); }\
								virtual const char* GetName() const override { return #type; }

#define EVENT_CLASS_CATEGORY(category) virtual int GetCategoryFlags() const override { return category; }

	// 事件基类
	class HAZEL_API Event
	{
		// EventDispatcher被指定为Event的友元类，可以访问Event的私有成员
		friend class EventDispatcher;
	public:
		virtual EventType GetEventType() const = 0;
		virtual const char* GetName() const = 0;
		virtual int GetCategoryFlags() const = 0;
		// 主要用于调试，将事件转换为字符串
		virtual std::string ToString() const { return GetName(); } 

		//判断是否是某个事件分类
		bool IsInCategory(EventCategory category)
		{
			return GetCategoryFlags() & category;
		}
	protected:
		// 事件是否被处理
		bool m_Handled = false; 
	};

	class EventDispatcher
	{
		// 这里是一个模板函数，它接受一个函数作为参数，该函数接受一个特定类型的事件作为参数，并返回一个布尔值。
		template<typename T>
		using EventFn = std::function<bool(T&)>;
	public:
		EventDispatcher(Event& event) : m_Event(event){ }

		template<typename T>
		bool Dispatch(EventFn<T> func)
		{
			if (m_Event.GetEventType() == T::GetStaticType())
			{
				m_Event.m_Handled = func(*(T*)&m_Event);
				return true;
			}
			return false;
		}
	private:
		Event& m_Event;
	};

	inline std::ostream& operator<<(std::ostream& os, const Event& e)
	{
		return os << e.ToString();
	}
}
