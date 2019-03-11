#pragma once

#include "Engine/Core.h"
#include "Model.h"

namespace Engine {
	class ENGINE_API Entity
	{
	public:
		Entity(Model* model, glm::vec3 rotation, glm::vec3 transition, glm::vec3 color, float scale)
			: m_Model(model), v_Rotation(rotation), v_Transition(transition), v_Color(color), f_Scale(scale)
		{}
		~Entity();

		void Draw();

		void setPosition(const glm::vec3 transition);
		glm::mat4 *UpdateWorldTransformation();
		glm::vec3 *GetTransition();
		Model *GetModel();

	private:
		Model * m_Model;
		glm::vec3 v_Rotation;
		glm::vec3 v_Transition;
		glm::vec3 v_Color;
		glm::mat4 m_Transformation;
		float f_Scale;


	};
}