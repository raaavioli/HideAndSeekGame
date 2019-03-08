#include "Layer.h"

namespace Engine {

	Layer::Layer(LayerType type, const std::string& name)
	{
		m_DebugName = name;
		m_Type = type;
	}


	Layer::~Layer()
	{
	}

}