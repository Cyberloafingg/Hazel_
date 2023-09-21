#include "hzpch.h"
#include "LayerStack.h"

namespace Hazel {

	LayerStack::LayerStack()
	{
		m_LayerInsert = m_Layers.begin();
	}

	LayerStack::~LayerStack()
	{
		for (Layer* layer : m_Layers)
			delete layer;
	}

	// ��layer���뵽��ǰlayer֮ǰ
	void LayerStack::PushLayer(Layer* layer)
	{
		// emplace����m_LayerInsert֮ǰ����layer
		// �ٸ����ӣ�����m_LayerInsertָ���һ��layer����ôemplace�����ڵ�һ��layer֮ǰ����layer
		m_LayerInsert = m_Layers.emplace(m_LayerInsert, layer);
	}

	// ��layer���뵽��ǰlayer֮��
	void LayerStack::PushOverlay(Layer* overlay)
	{
		// emplace_back����m_LayerInsert֮�����layer
		// �ٸ����ӣ�����m_LayerInsertָ���һ��layer����ôemplace_back�����ڵ�һ��layer֮�����layer
		m_Layers.emplace_back(overlay);
	}


	// ��layer��layerstack���Ƴ�
	void LayerStack::PopLayer(Layer* layer)
	{
		auto it = std::find(m_Layers.begin(), m_Layers.end(), layer);
		if (it != m_Layers.end())
		{
			m_Layers.erase(it);
			m_LayerInsert--;
		}
	}

	// ��overlay��layerstack���Ƴ���overlay����layer֮�ϵ�
	void LayerStack::PopOverlay(Layer* overlay)
	{
		auto it = std::find(m_Layers.begin(), m_Layers.end(), overlay);
		if (it != m_Layers.end())
			m_Layers.erase(it);
	}

}