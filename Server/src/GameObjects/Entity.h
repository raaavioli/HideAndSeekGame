#pragma once

#include <stdlib.h>

#include "glm/glm/glm.hpp"

#include "OBJLoader.h"
#include "Protocol.h"

#include "GameObjects/Collision/BoundingBox.h"


class Entity
{
public:
	Entity(BoundingBox *collider, glm::vec3 rotation, glm::vec3 transition, glm::vec3 color, float scale)
		: m_ColliderBox(collider), v_Rotation(rotation), v_Transition(transition), v_Scale(scale)
	{
		Update();
	}

	Entity(const char* modelName, bool normalize, bool center);
	~Entity();

	virtual const std::string &ToProtocolString() { return ""; };

	void Update();
	inline void SetPosition(const glm::vec3 transition) { v_Transition = transition; }
	inline void SetVelocity(const glm::vec3 velocity) { v_Velocity = velocity; }
	inline void DoScale(const float s) { v_Scale *= s; };
	inline void DoScale(const glm::vec3 &s) { v_Scale *= s; };
	inline void SetId(const int id) { m_Id = id; };
	virtual void Move() { v_Transition += v_Velocity; };
	bool CollidesWith(Entity &other);
	glm::mat4 *UpdateWorldTransformation();
	glm::vec3 *GetTransition();
	inline int GetId() { return m_Id; }
	inline glm::vec3 &GetScale() { return v_Scale; }
	inline glm::vec3 &GetPosition() { return v_Transition; }
	inline glm::vec3 &GetVelocity() { return v_Velocity; }
	inline std::string &GetModelName() { return m_ModelName; }
	inline BoundingBox &GetColliderBox() { return *m_ColliderBox; };

protected:
	int m_Id;
	glm::vec3 v_Rotation;
	glm::vec3 v_Transition;
	glm::vec3 v_Scale;
	glm::vec3 v_Velocity;
	glm::mat4 m_Transformation;
	BoundingBox *m_ColliderBox;
	std::string m_ModelName;
	std::string m_ProtocolString;
private:

};
