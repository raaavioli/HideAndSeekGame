#include "entity.h"

#include "glm/gtc/matrix_transform.hpp"
#include "Engine/Renderer/ShaderProgram.h"

namespace Engine {

	Entity::~Entity() {
	}

	void Entity::Draw() {
		ShaderProgram::BindVertexColor(&v_Color);
		// Temporary -- Should not be updated during the render cycle
		UpdateWorldTransformation();
		ShaderProgram::BindEntityWorldMatrix(&m_Transformation);
		m_Model->Draw();
	}

	glm::mat4 *Entity::UpdateWorldTransformation() {
		m_Transformation = glm::mat4(1.0f);
		m_Transformation = glm::translate(m_Transformation, v_Transition);
		m_Transformation = glm::rotate(m_Transformation, v_Rotation.x, glm::vec3(1.0f, 0.0f, 0.0f));
		m_Transformation = glm::rotate(m_Transformation, v_Rotation.y, glm::vec3(0.0f, 1.0f, 0.0f));
		m_Transformation = glm::rotate(m_Transformation, v_Rotation.z, glm::vec3(0.0f, 0.0f, 1.0f));
		m_Transformation = glm::scale(m_Transformation, glm::vec3(f_Scale));
		return &m_Transformation;
	}

	glm::vec3 *Entity::GetTransition() {
		return &v_Transition;
	}

	void Entity::setPosition(const glm::vec3 transition) {
		v_Transition = transition;
	}

	Model *Entity::GetModel()
	{
		return m_Model;
	}
}