#include "Player.h"

#include "OBJLoader.h"

Player::Player()
	: Entity(OBJLoader::GetAABB("monkey", true, true))
{
	float charScale = 3;
	DoScale(charScale);
	float depth = charScale * ((AABB*)m_ColliderBox)->GetColliderMax().z;
	SetPosition(glm::vec3(-27.5, -18.5, depth));
}


Player::~Player()
{
}

void Player::Move(unsigned int directions, float speed)
{
	static auto inDir = [&](Player::Direction d) { return directions & d; };

	glm::vec3 direction(0.0);
	if (inDir(Player::Direction::FORWARD))
		direction.y += 1;
	if (inDir(Player::Direction::BACKWARD))
		direction.y -= 1;
	if (inDir(Player::Direction::RIGHT))
		direction.x += 1;
	if (inDir(Player::Direction::LEFT))
		direction.x -= 1;

	SetVelocity(speed * direction);
}

const std::string &Player::ToProtocolString()
{
	return "";
};