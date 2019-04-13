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

const std::string Entity::ToProtocolString(InstructionType it)
{
	pChar n{ 5 };
	pInt i{ GetId() };
	glm::vec3 &entity_scale = GetScale();
	pVector3 s{ entity_scale.x, entity_scale.y, entity_scale.z };
	glm::vec3 &entity_pos = GetPosition();
	pVector3 p{ entity_pos.x, entity_pos.y, entity_pos.z };
	glm::vec3 &entity_color = GetColor();
	pVector3 c{ entity_color.x, entity_color.y, entity_color.z };
	pString64 m;
	strcpy(m.Message, m_ModelName.c_str());

	std::string m_ProtocolString;
	m_ProtocolString.append(Protocol::Stringify(it, Attribute::NUMATTRIBS, &n));
	m_ProtocolString.append(Protocol::Stringify(it, Attribute::ID, &i));
	m_ProtocolString.append(Protocol::Stringify(it, Attribute::POSITION, &p));
	m_ProtocolString.append(Protocol::Stringify(it, Attribute::SCALE, &s));
	m_ProtocolString.append(Protocol::Stringify(it, Attribute::COLOR, &c));
	m_ProtocolString.append(Protocol::Stringify(it, Attribute::MODEL, &m));

	return m_ProtocolString;
}

void Entity::Update()
{
	UpdateWorldTransformation();
	m_ColliderBox->Update(m_Transformation);
}

float Entity::GetHeight()
{
	return (((AABB*)m_ColliderBox)->GetColliderMax() - ((AABB*)m_ColliderBox)->GetColliderMin()).z;
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

