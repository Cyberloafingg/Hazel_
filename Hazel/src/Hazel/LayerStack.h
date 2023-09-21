#pragma once

#include "Hazel/Core.h"
#include "Layer.h"

#include <vector>

namespace Hazel {

	class HAZEL_API LayerStack
	{
	public:
		LayerStack();
		~LayerStack();

		// 添加layer到layerstack中，位置在最后一个layer的后面
		void PushLayer(Layer* layer);
		void PushOverlay(Layer* overlay);
		void PopLayer(Layer* layer);
		void PopOverlay(Layer* overlay);

		std::vector<Layer*>::iterator begin() { return m_Layers.begin(); }
		std::vector<Layer*>::iterator end() { return m_Layers.end(); }
	private:
		// 存储所有的layer，vector头部是push进去的layer，尾部是pushoverlay进去的layer
		std::vector<Layer*> m_Layers;
		// 指向vector的第一个元素
		std::vector<Layer*>::iterator m_LayerInsert;
	};

}