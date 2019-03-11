#pragma once

#include "Engine/Core.h"
#include "Model.h"

namespace Engine {
	class ENGINE_API Entity
	{
	public:
		Entity(Model* model, glm::vec3 rotation, glm::vec3 transition, float scale)
			: m_Model(model), v_Rotation(rotation), v_Transition(transition), f_Scale(scale)
		{}
		~Entity();
		void draw();

		void setPosition(const glm::vec3 transition);
		glm::mat4 GetWorldTransformation();
		glm::vec3 GetTransition();
		Model * GetModel();

	private:
		Model * m_Model;
		glm::vec3 v_Rotation;
		glm::vec3 v_Transition;
		float f_Scale;


	};
}