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
		v_Color			= glm::vec3(1.0, 1.0, 1.0);
		v_Scale			= glm::vec3(1.0, 1.0, 1.0);
		v_Velocity		= glm::vec3(0.0, 0.0, 0.0);
		m_IsVisible		= 1;

		Update();
	}

	Entity::Entity()
		: m_Model(OBJLoader::GetModel("unitcube", true, true))
	{
		m_ColliderBox = new AABB(m_Model->GetMinPos(), m_Model->GetMaxPos());
		v_Rotation = glm::vec3(0.0, 0.0, 0.0);
		v_Transition = glm::vec3(0.0, 0.0, 0.0);
		v_Color = glm::vec3(1.0, 1.0, 1.0);
		v_Scale = glm::vec3(1.0, 1.0, 1.0);
		v_Velocity = glm::vec3(0.0, 0.0, 0.0);
		m_IsVisible = 1;

		Update();
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
		if (m_IsVisible)
		{
			ShaderProgram::BindVertexColor(&v_Color);
			ShaderProgram::BindEntityWorldMatrix(&m_Transformation);
			m_Model->DrawModel();
			//Enable if we want to draw collision boxes. For debug.
			//m_ColliderBox->Draw();
		}
	}

	void Entity::UpdateModel(const char * modelname)
	{
		m_Model = OBJLoader::GetModel(modelname, true, true);
		delete m_ColliderBox;
		m_ColliderBox = new AABB(m_Model->GetMinPos(), m_Model->GetMaxPos());
	}

	glm::mat4 *Entity::UpdateWorldTransformation() 
	{
		m_Transformation = glm::mat4(1.0f);
		m_Transformation = glm::translate(m_Transformation, v_Transition);
		m_Transformation = glm::rotate(m_Transformation, v_Rotation.x, glm::vec3(1.0f, 0.0f, 0.0f));
		m_Transformation = glm::rotate(m_Transformation, v_Rotation.y, glm::vec3(0.0f, 1.0f, 0.0f));
		m_Transformation = glm::rotate(m_Transformation, v_Rotation.z, glm::vec3(0.0f, 0.0f, 1.0f));
		m_Transformation = glm::scale(m_Transformation, v_Scale);
		return &m_Transformation;
	}

	glm::vec3 *Entity::GetTransition() 
	{
		return &v_Transition;
	}

}