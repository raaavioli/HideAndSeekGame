#pragma once
#include "Core.h"

#include "Engine/Event/Event.h"
#include "Renderer/Renderer.h"
#include "Objects/Model.h"

namespace Engine {

	class ENGINE_API Layer
	{
	public:
		enum LayerType {
			LAYER,
			OVERLAY
		};
	public:
		Layer(LayerType type, std::vector<Model*> &objects, const std::string& name = "DafaultLayerName")
			: m_Objects(objects), m_DebugName(name), m_Type(type) {};
		virtual ~Layer();

		virtual void OnAttach() {}
		virtual void OnDetach() {}
		virtual void OnUpdate() {}
		virtual void OnEvent(Event &e) {}
		virtual void OnRender() { Renderer::GetRenderer().Render(m_Objects); }

		inline void PushModel(Model* m) { m_Objects.push_back(m); }
		inline bool isType(LayerType t) { return t == m_Type; }
		inline const std::string& GetName() const { return m_DebugName; }
		inline std::vector<Model*> &GetObjects() { return m_Objects;  }
		
	private:
		std::string m_DebugName;
		LayerType m_Type;
		std::vector<Model*> &m_Objects;
	};

}