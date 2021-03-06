#pragma once
#include "Core.h"

#include "Engine/Event/Event.h"
#include "Renderer/Renderer.h"
#include "Objects/Entity.h"

namespace Engine {

	class ENGINE_API Layer
	{
	public:
		enum LayerType {
			LAYER,
			OVERLAY
		};
	public:
		Layer(LayerType type, std::vector<Entity*> &objects, const std::string& name = "DafaultLayerName")
			: m_Objects(objects), m_DebugName(name), m_Type(type) {};

		Layer(LayerType type, const std::string& name = "DafaultLayerName")
			: m_DebugName(name), m_Type(type) 
		{
			m_Objects = std::vector<Engine::Entity*>();
		};
		virtual ~Layer();

		virtual void OnAttach() {}
		virtual void OnDetach() {}
		virtual void OnUpdate() {}
		virtual void OnEvent(Event &e) {}
		virtual void OnRender() { Renderer::GetRenderer().Render(m_Objects); }

		inline void PushModel(Entity* m) { m_Objects.push_back(m); }
		inline bool isType(LayerType t) { return t == m_Type; }
		inline const std::string& GetName() const { return m_DebugName; }
		inline std::vector<Entity*> &GetObjects() { return m_Objects;  }
		
	protected:
		std::vector<Entity*> m_Objects;
		LayerType m_Type;
		std::string m_DebugName;
	private:
	};

}