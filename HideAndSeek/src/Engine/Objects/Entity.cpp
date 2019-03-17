#include "entity.h"

#include "glm/gtc/matrix_transform.hpp"
#include "Engine/Renderer/ShaderProgram.h"

namespace Engine {
	
	Entity::Entity(Model * model)
		: m_Model(model)
	{
		m_ColliderBox	= new AABB(model->GetMinPos(), model->GetMaxPos());
		v_Rotation		= glm::vec3(0.0, 0.0, 0.0);
		v_Transition	= glm::vec3(0.0, 0.0, 0.0);
		v_Color			= glm::vec3(1.0, 0.0, 1.0);
		f_Scale			= 1.0;
	}

	Entity::~Entity() {
	}

	void Entity::Update()
	{
		UpdateWorldTransformation();
		m_ColliderBox->Update(m_Transformation);
	}

	bool Entity::CollidesWith(Entity &other)
	{
		return m_ColliderBox->CollidesWith(other.GetColliderBox());
	}

	void Entity::Draw()
	{
		ShaderProgram::BindVertexColor(&v_Color);
		ShaderProgram::BindEntityWorldMatrix(&m_Transformation);
		m_ColliderBox->Draw();
		m_Model->DrawModel();
	}

	glm::mat4 *Entity::UpdateWorldTransformation() 
	{
		m_Transformation = glm::mat4(1.0f);
		m_Transformation = glm::translate(m_Transformation, v_Transition);
		m_Transformation = glm::rotate(m_Transformation, v_Rotation.x, glm::vec3(1.0f, 0.0f, 0.0f));
		m_Transformation = glm::rotate(m_Transformation, v_Rotation.y, glm::vec3(0.0f, 1.0f, 0.0f));
		m_Transformation = glm::rotate(m_Transformation, v_Rotation.z, glm::vec3(0.0f, 0.0f, 1.0f));
		m_Transformation = glm::scale(m_Transformation, glm::vec3(f_Scale));
		return &m_Transformation;
	}

	glm::vec3 *Entity::GetTransition() 
	{
		return &v_Transition;
	}

	void Entity::SetPosition(const glm::vec3 transition) 
	{
		v_Transition = transition;
	}

	void Entity::Scale(float s)
	{
		f_Scale *= s;
	}

	void Entity::Move(glm::vec3 directions, float speed)
	{
		directions.x = abs(directions.x) < 0.001 ? 0 : speed * directions.x / abs(directions.x);
		directions.y = abs(directions.y) < 0.001 ? 0 : speed * directions.y / abs(directions.y);
		directions.z = abs(directions.z) < 0.001 ? 0 : speed * directions.z / abs(directions.z);

		v_Transition += directions;
	}

}