#include "Item.h"

Item::Item(const char* model, float weight, char status)
	: Entity(model, true, true), m_Status(status), m_Weight(weight), m_SecondsCarried(0)
{
	v_Scale = glm::vec3(3, 3, 3);
	v_Color = glm::vec3(1, 1, 1);
}

Item::~Item()
{
}
