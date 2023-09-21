#pragma once

#include "Hazel/Core.h"
#include "Hazel/Events/Event.h"

namespace Hazel {

	class HAZEL_API Layer
	{
	public:
		Layer(const std::string& name = "Layer");
		virtual ~Layer();

		// OnAttach()是在Layer被添加到LayerStack调用的
		virtual void OnAttach() {}
		// OnDetach()是在Layer被从LayerStack中移除调用的
		virtual void OnDetach() {}
		// OnUpdate()是在每一帧调用的
		virtual void OnUpdate() {}
		// OnEvent() 是在事件被分发的时候调用的
		virtual void OnEvent(Event& event) {}

		const std::string& GetName() const { return m_DebugName; }
	protected:
		// 为了方便调试，给每个Layer都加上一个名字
		std::string m_DebugName;
	};

}