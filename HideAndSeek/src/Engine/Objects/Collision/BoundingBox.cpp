#include "BoundingBox.h"

#include "Engine/Log.h"

namespace Engine {


	Engine::BoundingBox::~BoundingBox()
	{
	}


	//--------------------------------------------------------//
	//  AABB                                                  //
	//--------------------------------------------------------//
	
	Model* AABB::AABBModel = nullptr;

	AABB::~AABB()
	{
	}

	/* * * * *
	* Creates a unit cube to display as the bounding box outline
	* Cube has to be transformed prior to being rendered into
	* the scale and size it is represented
	*
	*
	*
	*
	*
	*/
	Model *Engine::AABB::GetStaticUnitModel()
	{
		if(!AABBModel)
		{

			const float unit_cube[24] = {
				-0.5f, -0.5f, -0.5f,
				-0.5f, -0.5f, 0.5f,
				-0.5f, 0.5f, -0.5f,
				-0.5f, 0.5f, 0.5f,
				0.5f, -0.5f, -0.5f,
				0.5f, -0.5f, 0.5f,
				0.5f, 0.5f, -0.5f,
				0.5f, 0.5f, 0.5f,
			};

			const unsigned int index_array[] = {
				1,0,2, // x negative
				2,3,1,
				1,0,4, // y negative
				4,5,1,
				4,0,2, // z negative
				2,6,4,
				5,4,6, // x positive
				6,7,5,
				3,2,6, // y positive
				6,7,3,
				5,1,3, // z positive
				3,7,5,
			};

			std::vector<float> cube_vector;
			for (int i = 0; i < 24; i++) {
				cube_vector.push_back(unit_cube[i]);
			}

			std::vector<unsigned int> index_vector;
			for (int i = 0; i < 36; i++) {
				index_vector.push_back(index_array[i]);
			}

			VAO *vao = new VAO();
			VBO *vertices = new VBO(&cube_vector, 3);
			VBO *indices = new VBO(&index_vector);

			AABBModel = new Model(vao, vertices, nullptr, nullptr, indices);
			return AABBModel;
		}	

		return AABBModel;
	}

}