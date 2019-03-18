#include "Player.h"



Player::Player()
	: Entity(Engine::OBJLoader::GetModel("monkey", true, true))
{
	float charScale = 4;
	Scale(charScale);
	float depth = charScale * m_Model->GetMaxPos().z;
	SetPosition(glm::vec3(0, 0, depth));
}


Player::~Player()
{
}

void Player::Move(unsigned int directions, float speed)
{
	using Direction = Engine::Direction;
	static auto inDir = [&](Direction d) { return directions & d; };

	glm::vec3 direction(0.0);
	if (inDir(Direction::FORWARD))
		direction.y += 1;
	if (inDir(Direction::BACKWARD))
		direction.y -= 1;
	if (inDir(Direction::RIGHT))
		direction.x += 1;
	if (inDir(Direction::LEFT))
		direction.x -= 1;

	SetVelocity(speed * direction);
}