#pragma once

#include <stdlib.h>

#include "Engine/Core.h"
#include "Model.h"
#include "OBJLoader.h"
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
		void UpdateModel(const char* modelname);
		inline void SetPosition(const glm::vec3 transition) { v_Transition = transition; }
		inline void SetVelocity(const glm::vec3 velocity) { v_Velocity = velocity; }
		inline void SetRotation(const glm::vec3 rotation) { v_Rotation = rotation; }
		inline void SetScale(const glm::vec3 scale) { v_Scale = scale; }
		inline void DoScale(const float s) { v_Scale *= s; };
		inline void DoScale(const glm::vec3 &s) { v_Scale *= s; };
		inline void SetId(const int id) { m_Id = id; }
		inline void Move() { v_Transition += v_Velocity; };
		bool CollidesWith(Entity &other);
		glm::vec3 *GetTransition();
		glm::mat4 *UpdateWorldTransformation();
		inline Model &GetModel() { return *m_Model; };
		inline int GetId() { return m_Id; }
		inline glm::vec3 &GetScale() { return v_Scale; }
		inline glm::vec3 &GetPosition() { return v_Transition; }
		inline glm::vec3 &GetVelocity() { return v_Velocity; }
		inline BoundingBox &GetColliderBox() { return *m_ColliderBox; };

	protected:
		Model * m_Model;
		BoundingBox *m_ColliderBox;
		glm::vec3 v_Color;
	private:
		int m_Id;
		glm::vec3 v_Rotation;
		glm::vec3 v_Transition;
		glm::vec3 v_Scale;
		glm::vec3 v_Velocity;
		glm::mat4 m_Transformation;


	};
}