#include "OBJLoader.h"
#include <iostream>
#include <algorithm>

#pragma warning(disable:4996)

OBJLoader* OBJLoader::s_Instance= nullptr;

std::map<std::string, BoundingBox*> OBJLoader::boundingBoxCache;

OBJLoader::~OBJLoader()
{
}

void OBJLoader::Init()
{
	if(!s_Instance) 
	{
		s_Instance = new OBJLoader();

		boundingBoxCache = std::map<std::string, BoundingBox*>();
	}
}

void OBJLoader::Shutdown() {
	for (auto &[key, value] : boundingBoxCache)
		delete value;
}

BoundingBox *OBJLoader::GetAABB(const char* filename, bool normalize, bool centralize) {
	std::map<std::string, BoundingBox*>::const_iterator iter = boundingBoxCache.find(std::string(filename));

	if (iter != boundingBoxCache.end()) {
		return (*iter).second;
	} 
	else 
		return loadObjFile(filename, normalize, centralize);
}

BoundingBox *OBJLoader::loadObjFile(const char* filename, bool normalize, bool centralize) {
	std::string fn = "src/GameObjects/objects/" + std::string(filename) + ".obj";
	FILE* file = fopen(fn.c_str(), "r");
	errno = 0;

	if (file == NULL)
	{
		std::cout << "Filename: " << fn.c_str() << std::endl;
		std::cerr << "File not found" << std::endl;
	}
	std::vector<glm::vec3> temp_vertices;

	glm::vec3 v_MaxCoords = glm::vec3(0.0);
	glm::vec3 v_MinCoords = glm::vec3(0.0);

	while (1) {
		char line_head[128];
		int res = fscanf(file, "%s", line_head);
		if (res == EOF)
			break;

		if (strcmp(line_head, "v") == 0) {
			glm::vec3 vertex;
			fscanf_s(file, "%f %f %f\n", &vertex.x, &vertex.y, &vertex.z);

			AddMaxVertex(v_MaxCoords, vertex);
			AddMinVertex(v_MinCoords, vertex);

			temp_vertices.push_back(vertex);
		}
	}
	std::fclose(file);

	glm::vec3 v_MaxTemp(0.0);
	glm::vec3 v_MinTemp(0.0);
	if (centralize || normalize)
	{
		float dx = v_MaxCoords.x - v_MinCoords.x;
		float dy = v_MaxCoords.y - v_MinCoords.y;
		float dz = v_MaxCoords.z - v_MinCoords.z;

		float max_dist = std::max(dx, std::max(dy, dz));

		float x_center = (v_MaxCoords.x + v_MinCoords.x) / 2;
		float y_center = (v_MaxCoords.y + v_MinCoords.y) / 2;
		float z_center = (v_MaxCoords.z + v_MinCoords.z) / 2;

		glm::vec3 v_Center(x_center, y_center, z_center);
			
		for (int i = 0; i < temp_vertices.size(); i++) {
			if (centralize) {
				temp_vertices.at(i) -= v_Center;
			}
			if (normalize) {
				temp_vertices.at(i) /= max_dist;
			}

			AddMaxVertex(v_MaxTemp, temp_vertices.at(i));
			AddMinVertex(v_MinTemp, temp_vertices.at(i));
		}
	}

	if (normalize || centralize) {
		v_MaxCoords = v_MaxTemp;
		v_MinCoords = v_MinTemp;
	}

	BoundingBox *b = new AABB(v_MaxCoords, v_MinCoords);
	boundingBoxCache.insert(std::make_pair(std::string(filename), b));
	return b;
}

void OBJLoader::AddMaxVertex(glm::vec3 &v_max, glm::vec3 &v_other) {
	v_max.x = std::max(v_max.x, v_other.x);
	v_max.y = std::max(v_max.y, v_other.y);
	v_max.z = std::max(v_max.z, v_other.z);
}

void OBJLoader::AddMinVertex(glm::vec3 &v_max, glm::vec3 &v_other) {
	v_max.x = std::min(v_max.x, v_other.x);
	v_max.y = std::min(v_max.y, v_other.y);
	v_max.z = std::min(v_max.z, v_other.z);
}



