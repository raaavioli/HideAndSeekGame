#pragma once

#include <stdlib.h>

#include "Engine/Core.h"
#include "Model.h"
#include "Collision/BoundingBox.h"

namespace Engine {
	class ENGINE_API Entity
	{
	public:
		Entity(Model* model, BoundingBox *collider, glm::vec3 rotation, glm::vec3 transition, glm::vec3 color, float scale)
			: m_Model(model), m_ColliderBox(collider), v_Rotation(rotation), v_Transition(transition), v_Color(color), f_Scale(scale)
		{}

		Entity(Model* model);
		~Entity();

		void Update();
		void Draw();
		void SetPosition(const glm::vec3 transition);
		void Scale(float s);
		void Move(glm::vec3 directions, float speed);
		bool CollidesWith(Entity &other);
		glm::mat4 *UpdateWorldTransformation();
		glm::vec3 *GetTransition();
		inline Model &GetModel() { return *m_Model; };
		inline BoundingBox &GetColliderBox() { return *m_ColliderBox; };

	private:
		Model * m_Model;
		BoundingBox *m_ColliderBox;
		glm::vec3 v_Rotation;
		glm::vec3 v_Transition;
		glm::vec3 v_Color;
		glm::mat4 m_Transformation;
		float f_Scale;


	};
}