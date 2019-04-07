#include "Entity.h"

#include "glm/glm/gtc/matrix_transform.hpp"
	
Entity::Entity(const char* modelName, bool normalize, bool center)
	: m_ColliderBox(OBJLoader::GetAABB(modelName, normalize, center)),
	m_ModelName(modelName)
{
	v_Rotation		= glm::vec3(0.0, 0.0, 0.0);
	v_Transition	= glm::vec3(0.0, 0.0, 0.0);
	v_Scale			= glm::vec3(1.0, 1.0, 1.0);
	v_Velocity		= glm::vec3(0.0, 0.0, 0.0);

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

