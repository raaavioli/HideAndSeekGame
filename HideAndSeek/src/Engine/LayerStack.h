#pragma once

#include "Core.h"
#include "Layer.h"

namespace Engine {

	class ENGINE_API LayerStack
	{
	public:
		LayerStack();
		~LayerStack();

		void PushLayer(Layer* layer);
		void PopLayer(Layer* layer);
		void PushOverlay(Layer* overlay);
		void PopOverlay(Layer* overlay);

		inline std::vector<Layer*>::iterator StackBegin() { return m_LayerStack.begin(); }
		inline std::vector<Layer*>::iterator StackEnd() { return m_LayerStack.end(); }
	private:
		std::vector<Layer*> m_LayerStack;
		std::vector<Layer*>::iterator m_LayerOverlayDivision;
	};

}
