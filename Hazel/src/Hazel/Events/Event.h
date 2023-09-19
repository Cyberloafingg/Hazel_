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
	// Hazel�е��¼���ǰ�������ģ�����ζ�ŵ��¼�����ʱ���������������ɲ��ұ�����������
	// Ϊ�˽�����һ�����õĲ��Կ��������¼������л����¼������ڸ��½׶εġ��¼������ִ������ǡ�


	// �¼�����
	enum class EventType
	{
		None = 0,
		WindowClose, WindowResize, WindowFocus, WindowLostFocus, WindowMoved,
		AppTick, AppUpdate, AppRender,
		KeyPressed, KeyReleased,
		MouseButtonPressed, MouseButtonReleased, MouseMoved, MouseScrolled
	};

	// �¼����͵ķ���
	/*
	 * ���뼼�ɻ���λ���㣬��Ҫ���ڽ���ͬ�ı�־��״̬�����һ���Ա���һ�������б�ʾ���״̬��ѡ���ʡ�ڴ�����Ч�ʡ�
	   ��������һ���������� x������1���� x λ�����磬BIT(0) ������1��BIT(1) ������2��BIT(2) ������4���Դ����ơ�������λ�����������������ɾ��е���λ���õ�����ֵ��
	   ������ɵ���Ҫ�������ڣ������ʹ��λ���������һ�������Ƿ�����ض��ı�־���Լ�����־��ӵ�����������Ƴ������磬�����ʹ�ð�λ������ & �����һ���¼��Ƿ�����ĳ���ض����
	   ���磺if (event.GetCategoryFlags() & EventCategoryMouse) { ... } �����ж��Ƿ���������¼�
	   ���ߣ�����ʹ�ð�λ������ | ����һ���¼������������eventCategory |= EventCategoryKeyboard; // ���¼�������������
	   
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

// ����ĺ궨������������������������ʱ��ȡ�¼������͡����ƺ����
#define EVENT_CLASS_TYPE(type) static EventType GetStaticType() { return EventType::##type; }\
								virtual EventType GetEventType() const override { return GetStaticType(); }\
								virtual const char* GetName() const override { return #type; }

#define EVENT_CLASS_CATEGORY(category) virtual int GetCategoryFlags() const override { return category; }

	// �¼�����
	class HAZEL_API Event
	{
		// EventDispatcher��ָ��ΪEvent����Ԫ�࣬���Է���Event��˽�г�Ա
		friend class EventDispatcher;
	public:
		virtual EventType GetEventType() const = 0;
		virtual const char* GetName() const = 0;
		virtual int GetCategoryFlags() const = 0;
		// ��Ҫ���ڵ��ԣ����¼�ת��Ϊ�ַ���
		virtual std::string ToString() const { return GetName(); } 

		//�ж��Ƿ���ĳ���¼�����
		bool IsInCategory(EventCategory category)
		{
			return GetCategoryFlags() & category;
		}
	protected:
		// �¼��Ƿ񱻴���
		bool m_Handled = false; 
	};

	class EventDispatcher
	{
		// ������һ��ģ�庯����������һ��������Ϊ�������ú�������һ���ض����͵��¼���Ϊ������������һ������ֵ��
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
