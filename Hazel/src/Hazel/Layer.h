#pragma once

#include "Hazel/Core.h"
#include "Hazel/Events/Event.h"

namespace Hazel {

	class HAZEL_API Layer
	{
	public:
		Layer(const std::string& name = "Layer");
		virtual ~Layer();

		// OnAttach()����Layer����ӵ�LayerStack���õ�
		virtual void OnAttach() {}
		// OnDetach()����Layer����LayerStack���Ƴ����õ�
		virtual void OnDetach() {}
		// OnUpdate()����ÿһ֡���õ�
		virtual void OnUpdate() {}
		// OnEvent() �����¼����ַ���ʱ����õ�
		virtual void OnEvent(Event& event) {}

		const std::string& GetName() const { return m_DebugName; }
	protected:
		// Ϊ�˷�����ԣ���ÿ��Layer������һ������
		std::string m_DebugName;
	};

}