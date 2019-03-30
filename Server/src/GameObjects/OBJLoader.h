#pragma once

#include <map>

#include "glm/glm/glm.hpp"

#include "Collision/BoundingBox.h"

class OBJLoader
{
public:
	~OBJLoader();

	static void Init();
	static void Shutdown();

	static BoundingBox *GetAABB(const char* filename, bool normalize, bool centralize);

protected:
	static std::map<std::string, BoundingBox*> boundingBoxCache;
private:
	static BoundingBox *loadObjFile(const char* filename, bool normalized, bool centered);
	static void OBJLoader::AddMaxVertex(glm::vec3 &v_max, glm::vec3 &v_other);
	static void OBJLoader::AddMinVertex(glm::vec3 &v_max, glm::vec3 &v_other);

	static OBJLoader* s_Instance;
};

