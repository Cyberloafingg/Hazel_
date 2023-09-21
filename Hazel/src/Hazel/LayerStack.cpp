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

	// 将layer插入到当前layer之前
	void LayerStack::PushLayer(Layer* layer)
	{
		// emplace是在m_LayerInsert迭代器之前插入layer
		m_LayerInsert = m_Layers.emplace(m_LayerInsert, layer);
	}

	// 将layer插入到当前layer之后
	void LayerStack::PushOverlay(Layer* overlay)
	{
		// emplace_back是在m_LayerInsert之后插入layer
		m_Layers.emplace_back(overlay);
	}


	// 将layer从layerstack中移除
	void LayerStack::PopLayer(Layer* layer)
	{
		auto it = std::find(m_Layers.begin(), m_Layers.end(), layer);
		if (it != m_Layers.end())
		{
			m_Layers.erase(it);
			m_LayerInsert--;
		}
	}

	// 将overlay从layerstack中移除，overlay是在layer之上的
	void LayerStack::PopOverlay(Layer* overlay)
	{
		auto it = std::find(m_Layers.begin(), m_Layers.end(), overlay);
		if (it != m_Layers.end())
			m_Layers.erase(it);
	}

}