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

	virtual const std::string ToProtocolString(InstructionType it);

	void Update();
	inline void SetPosition(const glm::vec3 transition) { v_Transition = transition; }
	inline void SetVelocity(const glm::vec3 velocity) { v_Velocity = velocity; }
	inline void SetVisibility(const char visible) { m_IsVisible = visible; }
	inline void SetColor(const glm::vec3 color) { v_Color = color; }
	inline void DoScale(const float s) { v_Scale *= s; };
	inline void DoScale(const glm::vec3 &s) { v_Scale *= s; };
	inline void SetId(const int id) { m_Id = id; };
	inline bool IsFlying() { return abs(v_Velocity.z) > 0.02; }
	virtual void Move() { v_Transition += v_Velocity; };
	float GetHeight();
	bool CollidesWith(Entity &other);
	glm::mat4 *UpdateWorldTransformation();
	glm::vec3 *GetTransition();
	inline int GetId() { return m_Id; }
	inline glm::vec3 &GetScale() { return v_Scale; }
	inline glm::vec3 &GetColor() { return v_Color; }
	inline glm::vec3 &GetPosition() { return v_Transition; }
	inline glm::vec3 &GetVelocity() { return v_Velocity; }
	inline char GetVisibility() { return m_IsVisible; }
	inline std::string &GetModelName() { return m_ModelName; }
	inline BoundingBox &GetColliderBox() { return *m_ColliderBox; };

protected:
	int m_Id;
	char m_IsVisible;
	glm::vec3 v_Rotation;
	glm::vec3 v_Transition;
	glm::vec3 v_Scale;
	glm::vec3 v_Velocity;
	glm::vec3 v_Color;
	glm::mat4 m_Transformation;
	BoundingBox *m_ColliderBox;
	std::string m_ModelName;
private:

};
