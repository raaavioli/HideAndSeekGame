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
			: m_Model(model), m_ColliderBox(collider), v_Rotation(rotation), v_Transition(transition), v_Color(color), v_Scale(scale)
		{
			Update();
		}

		Entity(Model* model);
		~Entity();

		void Update();
		void Draw();
		inline void SetPosition(const glm::vec3 transition) { v_Transition = transition; }
		inline void SetVelocity(const glm::vec3 velocity) { v_Velocity = velocity; }
		inline void Scale(const float s) { v_Scale *= s; };
		inline void Scale(const glm::vec3 &s) { v_Scale *= s; };
		inline void Move() { v_Transition += v_Velocity; };
		bool CollidesWith(Entity &other);
		glm::mat4 *UpdateWorldTransformation();
		glm::vec3 *GetTransition();
		inline Model &GetModel() { return *m_Model; };
		inline glm::vec3 GetPosition() { return v_Transition; }
		inline glm::vec3 &GetVelocity() { return v_Velocity; }
		inline BoundingBox &GetColliderBox() { return *m_ColliderBox; };

	protected:
		Model * m_Model;
		BoundingBox *m_ColliderBox;
	private:
		glm::vec3 v_Rotation;
		glm::vec3 v_Transition;
		glm::vec3 v_Color;
		glm::vec3 v_Scale;
		glm::vec3 v_Velocity;
		glm::mat4 m_Transformation;


	};
}