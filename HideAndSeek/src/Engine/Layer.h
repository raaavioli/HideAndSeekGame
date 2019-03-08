#pragma once
#include "Core.h"

#include "Engine/Event/Event.h"

namespace Engine {

	class ENGINE_API Layer
	{
	public:
		enum LayerType {
			LAYER,
			OVERLAY
		};
	public:
		Layer(LayerType type, const std::string& name = "DafaultLayerName");
		virtual ~Layer();

		virtual void OnAttach() {}
		virtual void OnDetach() {}
		virtual void OnUpdate() {}
		virtual void OnEvent(Event &e) {}

		inline bool isType(LayerType t) { return t == m_Type; }
		inline const std::string& GetName() const { return m_DebugName; }
	private:
		std::string m_DebugName;
		LayerType m_Type;
	};

}