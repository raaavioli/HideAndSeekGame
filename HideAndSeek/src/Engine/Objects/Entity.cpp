#include "entity.h"

#include "glm/gtc/matrix_transform.hpp"

namespace Engine {

	Entity::~Entity() {
	}

	void Entity::draw() {
		m_Model->Draw();
	}

	glm::mat4 Entity::GetWorldTransformation() {
		glm::mat4 transformation(1.0f);
		transformation = glm::translate(transformation, v_Transition);
		transformation = glm::rotate(transformation, v_Rotation.x, glm::vec3(1.0f, 0.0f, 0.0f));
		transformation = glm::rotate(transformation, v_Rotation.y, glm::vec3(0.0f, 1.0f, 0.0f));
		transformation = glm::rotate(transformation, v_Rotation.z, glm::vec3(0.0f, 0.0f, 1.0f));
		transformation = glm::scale(transformation, glm::vec3(f_Scale));
		return transformation;
	}

	glm::vec3 Entity::GetTransition() {
		return v_Transition;
	}

	void Entity::setPosition(const glm::vec3 transition) {
		v_Transition = transition;
	}

	Model * Entity::GetModel()
	{
		return m_Model;
	}
}