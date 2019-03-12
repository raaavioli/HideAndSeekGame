#pragma once

#include <glm/glm.hpp>

#include "Engine/Core.h"
#include "Engine/Log.h"
#include "Model.h"

namespace Engine {

	class ENGINE_API OBJLoader
	{
	public:
		~OBJLoader();

		static void Init();
		static void Shutdown();

		static Model *GetModel(const char* filename, bool normalize, bool center);

	protected:
		static std::vector<VAO*> loadedVAOs;
		static std::vector<VBO*> loadedVBOs;

		static std::vector<glm::vec3> vertices_;
		static std::vector<glm::vec3> normals_;
		static std::vector<glm::vec2> textures_;
		static std::vector<unsigned int> indices_;
	private:
		static Model *loadObjFile(const char* filename, bool normalized, bool centered);
		static void OBJLoader::AddMaxVertex(glm::vec3 &v_max, glm::vec3 &v_other);
		static void OBJLoader::AddMinVertex(glm::vec3 &v_max, glm::vec3 &v_other);

		static bool getSimilarVertexIndex(
			const glm::vec3 & vertex_in, const glm::vec3 & normal_in, const glm::vec2 & texture_in,
			const std::vector<glm::vec3> & out_vertices, const std::vector<glm::vec3> & out_normals, const std::vector<glm::vec2> & out_textures,
			unsigned int & index
		);

		static void generateIndexBuffer(std::vector<glm::uvec3> *vertex_index_data,
			std::vector<glm::vec3> *in_vertices,
			std::vector<glm::vec2> *in_textures,
			std::vector<glm::vec3> *in_normals
		);
		static bool isNear(float a, float b);

		static OBJLoader* s_Instance;
	};

}