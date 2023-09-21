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

		// ���layer��layerstack�У�λ�������һ��layer�ĺ���
		void PushLayer(Layer* layer);
		void PushOverlay(Layer* overlay);
		void PopLayer(Layer* layer);
		void PopOverlay(Layer* overlay);

		std::vector<Layer*>::iterator begin() { return m_Layers.begin(); }
		std::vector<Layer*>::iterator end() { return m_Layers.end(); }
	private:
		// �洢���е�layer��vectorͷ����push��ȥ��layer��β����pushoverlay��ȥ��layer
		std::vector<Layer*> m_Layers;
		// ָ��vector�ĵ�һ��Ԫ��
		std::vector<Layer*>::iterator m_LayerInsert;
	};

}