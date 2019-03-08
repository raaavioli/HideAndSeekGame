#include "LayerStack.h"


namespace Engine {

	LayerStack::LayerStack()
	{
		m_LayerOverlayDivision = StackBegin();
	}


	LayerStack::~LayerStack()
	{
		for (Layer* layer : m_LayerStack)
			delete layer;
	}

	void LayerStack::PushLayer(Layer* layer)
	{
		if(layer->isType(Layer::LAYER))
			m_LayerOverlayDivision = m_LayerStack.emplace(m_LayerOverlayDivision, layer);
	}

	void LayerStack::PopLayer(Layer* layer)
	{
		auto it = std::find(StackBegin(), StackEnd(), layer);
		
		if(it != StackEnd() && layer->isType(Layer::LAYER))
		{
			m_LayerStack.erase(it);
			m_LayerOverlayDivision--;
		}
	}

	void LayerStack::PushOverlay(Layer* overlay)
	{
		if(overlay->isType(Layer::OVERLAY))
			m_LayerStack.emplace_back(overlay);
	}

	void LayerStack::PopOverlay(Layer* overlay)
	{
		auto it = std::find(StackBegin(), StackEnd(), overlay);

		if (it != StackEnd() && overlay->isType(Layer::OVERLAY))
			m_LayerStack.erase(it);
	}

}