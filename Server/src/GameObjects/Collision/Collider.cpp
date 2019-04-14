#include "Collider.h"
#include "BoundingBox.h"

#include "GameObjects/Item.h"
#include "GameObjects/Player.h"

Collider *Collider::s_Instance = nullptr;
std::vector<Entity*> Collider::dynamicEntities;
std::vector<Entity*>  Collider::staticEntities;
std::vector<Entity*> Collider::lootableEntities;

void Collider::Init()
{
	s_Instance = new Collider();
	dynamicEntities = std::vector<Entity*>();
	staticEntities = std::vector<Entity*>();
	lootableEntities = std::vector<Entity*>();
}

void Collider::Shutdown()
{
	delete s_Instance;
}

void Collider::Interact()
{

	for (int i = 0; i < dynamicEntities.size(); i++) 
	{
		Player dynamicCopy = *((Player*)dynamicEntities.at(i));
		float x = dynamicCopy.GetVelocity().x;
		dynamicCopy.Move();
		dynamicCopy.Update();

		for (int j = 0; j < dynamicEntities.size(); j++) {
			if (i == j) continue;
			Player dynamicOtherCopy = *((Player*)dynamicEntities.at(j));
			dynamicOtherCopy.Move();
			//Probably don't want to update this for every object every time.
			dynamicOtherCopy.Update();
			
			if (dynamicCopy.GetAction() == ATTACK && !dynamicCopy.IsFlying() && !dynamicOtherCopy.IsFlying())
			{
				static int hitRange = 2; //Should not be hard-coded here
				if (glm::distance(dynamicCopy.GetPosition(), dynamicOtherCopy.GetPosition()) < hitRange)
				{
					((Player*)dynamicEntities.at(i))->Hit((Player*)dynamicEntities.at(j));
				}
			}

			if (dynamicCopy.CollidesWith(dynamicOtherCopy)) {
				if (dynamicCopy.GetColliderBox().GetColliderType() == tAABB
					&& dynamicOtherCopy.GetColliderBox().GetColliderType() == tAABB) {

					AABBvsAABB(&dynamicCopy, &dynamicOtherCopy);

					dynamicEntities.at(i)->SetVelocity(dynamicCopy.GetVelocity());
				}
			}
		}

		for (int j = 0; j < staticEntities.size(); j++) {
			Entity staticOther = *staticEntities.at(j);

			if (dynamicCopy.CollidesWith(staticOther)) {
				if (dynamicCopy.GetColliderBox().GetColliderType() == tAABB
					&& staticOther.GetColliderBox().GetColliderType() == tAABB) {

					AABBvsAABB(&dynamicCopy, &staticOther);
					dynamicEntities.at(i)->SetVelocity(dynamicCopy.GetVelocity());
				}
			}
		}


		dynamicEntities.at(i)->Move();
		dynamicEntities.at(i)->Update();

		for (Entity* lootable : lootableEntities) {
			Item* item = (Item*)lootable;
			Player* player = (Player*)dynamicEntities.at(i);
			if (player->GetAction() == PICKUP && !player->HasItem(item) && dynamicEntities.at(i)->CollidesWith(*lootable))
			{
				player->PushItem(item);
				player->SetAction(InstructionType::OBJERROR);
			}
			else if (player->GetAction() == ATTACK && player->HasItem(item) && item->GetModelName() == "wingboots")
				player->SetFlying();
		}
		((Player*)dynamicEntities.at(i))->SetAction(InstructionType::OBJERROR);
	}
}

/*
* Taken from https://gamedevelopment.tutsplus.com/tutorials/how-to-create-a-custom-2d-physics-engine-the-basics-and-impulse-resolution--gamedev-6331
*/
void Collider::AABBvsAABB(Entity *a, Entity *b)
{
	// Vector from A to B
	glm::vec3 v_Between = *b->GetTransition() - *a->GetTransition();

	AABB &abox = (AABB&) a->GetColliderBox();
	AABB &bbox = (AABB&) b->GetColliderBox();

	// Calculate half extents along x axis for each object
	float a_extent = (abox.GetColliderMax().x - abox.GetColliderMin().x) / 2;
	float b_extent = (bbox.GetColliderMax().x - bbox.GetColliderMin().x) / 2;

	// Calculate overlap on x axis
	float x_overlap = a_extent + b_extent - abs(v_Between.x);

	// SAT test on x axis
	if (x_overlap > 0)
	{
		// Calculate half extents along y axis for each object
		float a_extent = (abox.GetColliderMax().y - abox.GetColliderMin().y) / 2;
		float b_extent = (bbox.GetColliderMax().y - bbox.GetColliderMin().y) / 2;

		// Calculate overlap on y axis
		float y_overlap = a_extent + b_extent - abs(v_Between.y);

		// SAT test on y axis
		if (y_overlap > 0)
		{

			// Calculate half extents along z axis for each object
			float a_extent = (abox.GetColliderMax().z - abox.GetColliderMin().z) / 2;
			float b_extent = (bbox.GetColliderMax().z - bbox.GetColliderMin().z) / 2;

			// Calculate overlap on y axis
			float z_overlap = a_extent + b_extent - abs(v_Between.z);

			if (z_overlap > 0)
			{
				if (y_overlap > x_overlap) 
				{
					if (z_overlap > x_overlap)
					{
						a->GetVelocity() *= glm::vec3(0, 1, 1);
						b->GetVelocity() *= glm::vec3(0, 1, 1);
					}
					else 
					{
						a->GetVelocity() *= glm::vec3(1, 1, 0);
						b->GetVelocity() *= glm::vec3(1, 1, 0);
					}
				}
				else
				{
					if (z_overlap > y_overlap)
					{
						a->GetVelocity() *= glm::vec3(1, 0, 1);
						b->GetVelocity() *= glm::vec3(1, 0, 1);
					}
					else
					{
						a->GetVelocity() *= glm::vec3(1, 1, 0);
						b->GetVelocity() *= glm::vec3(1, 1, 0);
					}
				}
			}
		}
	}
}

void Collider::Add(Entity *entity, MovementType c_Type)
{
	if (c_Type == STATIC)
		staticEntities.push_back(entity);
	else if (c_Type == DYNAMIC)
		dynamicEntities.push_back(entity);
	else if (c_Type == LOOTABLE)
		lootableEntities.push_back(entity);
}

void Collider::Remove(Entity *entity)
{
		
}

