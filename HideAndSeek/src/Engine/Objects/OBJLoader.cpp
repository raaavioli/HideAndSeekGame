#include "OBJLoader.h"

#pragma warning(disable:4996)

namespace Engine {
	OBJLoader* OBJLoader::s_Instance= nullptr;

	std::vector<VAO*> OBJLoader::loadedVAOs;
	std::vector<VBO*> OBJLoader::loadedVBOs;

	std::vector<glm::vec3> OBJLoader::vertices_;
	std::vector<glm::vec3> OBJLoader::normals_;
	std::vector<glm::vec2> OBJLoader::textures_;
	std::vector<unsigned int> OBJLoader::indices_;

	OBJLoader::~OBJLoader()
	{
	}

	void OBJLoader::Init()
	{
		CORE_ASSERT(!s_Instance, "OBJLoader is already Initialized");
		s_Instance = new OBJLoader();

		loadedVAOs	= std::vector<VAO*>();
		loadedVBOs	= std::vector<VBO*>();

		vertices_	= std::vector<glm::vec3>();
		normals_	= std::vector<glm::vec3>();
		textures_	= std::vector<glm::vec2>();
		indices_	= std::vector<unsigned int>();
	}

	void OBJLoader::Shutdown() {
		for (VAO *vao : loadedVAOs) {
			delete vao;
		}

		for (VBO *vbo : loadedVBOs) {
			delete vbo;
		}
	}

	Model *OBJLoader::GetModel(const char* filename) {
		return loadObjFile(filename);
	}

	Model *OBJLoader::loadObjFile(const char* filename) {
		std::string fn = "src/Application/objects/" + std::string(filename) + ".obj";
		FILE* file = fopen(fn.c_str(), "r");
		errno = 0;

		CORE_ASSERT((file != NULL), "File not found");
		CORE_ERROR("Filename: '{0}' ", fn.c_str());

		std::vector<glm::uvec3> vertex_index_data;
		std::vector<glm::vec3> temp_vertices;
		std::vector<glm::vec3> temp_normals;
		std::vector<glm::vec2> temp_textures;

		while (1) {
			char line_head[128];
			int res = fscanf(file, "%s", line_head);
			if (res == EOF)
				break;

			if (strcmp(line_head, "v") == 0) {
				glm::vec3 vertex;
				fscanf_s(file, "%f %f %f\n", &vertex.x, &vertex.y, &vertex.z);
				temp_vertices.push_back(vertex);
			}
			else if (strcmp(line_head, "vn") == 0) {
				glm::vec3 normal;
				fscanf_s(file, "%f %f %f\n", &normal.x, &normal.y, &normal.z);
				temp_normals.push_back(normal);
			}
			else if (strcmp(line_head, "vt") == 0) {
				glm::vec2 texture;
				fscanf_s(file, "%f %f\n", &texture.x, &texture.y);
				temp_textures.push_back(glm::vec2(texture.x, 1 - texture.y));
			}
			else if (strcmp(line_head, "f") == 0) {
				glm::uvec3 vertex1;
				glm::uvec3 vertex2;
				glm::uvec3 vertex3;


				fscanf(file, "%d/%d/%d %d/%d/%d %d/%d/%d",
					&vertex1.x, &vertex1.y, &vertex1.z,
					&vertex2.x, &vertex2.y, &vertex2.z,
					&vertex3.x, &vertex3.y, &vertex3.z
				);

				vertex_index_data.push_back(vertex1);
				vertex_index_data.push_back(vertex2);
				vertex_index_data.push_back(vertex3);
			}
		}

		generateIndexBuffer(&vertex_index_data, &temp_vertices, &temp_textures, &temp_normals);
		std::fclose(file);


		VAO *vao = new VAO();
		VBO *vertexBuffer = new VBO(&vertices_);
		VBO *normalBuffer = new VBO(&normals_);
		VBO *textureBuffer = new VBO(&textures_);
		VBO *indexBuffer = new VBO(&indices_);
	
		return new Model(vao, vertexBuffer, normalBuffer, textureBuffer, indexBuffer);
	}

	bool OBJLoader::isNear(float a, float b) {
		return fabs(a - b) < 0.01;
	}

	bool OBJLoader::getSimilarVertexIndex(
		const glm::vec3 & vertex_in, const glm::vec3 & normal_in, const glm::vec2 & texture_in,
		const std::vector<glm::vec3> & out_vertices, const std::vector<glm::vec3> & out_normals, const std::vector<glm::vec2> & out_textures,
		unsigned int & index) {

		for (unsigned int i = 0; i < out_vertices.size(); i++) {
			if (isNear(vertex_in.x, out_vertices[i].x) &&
				isNear(vertex_in.y, out_vertices[i].y) &&
				isNear(vertex_in.z, out_vertices[i].z) &&
				isNear(normal_in.x, out_normals[i].x) &&
				isNear(normal_in.y, out_normals[i].y) &&
				isNear(normal_in.z, out_normals[i].z) &&
				isNear(texture_in.x, out_textures[i].x) &&
				isNear(texture_in.y, out_textures[i].y)
				) {
				index = i;
				return true;
			}
		}

		return false;
	}

	void OBJLoader::generateIndexBuffer(
		std::vector<glm::uvec3> *vertex_index_data,
		std::vector<glm::vec3> *in_vertices,
		std::vector<glm::vec2> *in_textures,
		std::vector<glm::vec3> *in_normals
	) {
		vertices_.clear();
		textures_.clear();
		normals_.clear();
		indices_.clear();

		for (unsigned int i = 0; i < vertex_index_data->size(); i++) {
			unsigned int vertex_index = vertex_index_data->at(i).x - 1;
			unsigned int texture_index = vertex_index_data->at(i).y - 1;
			unsigned int normal_index = vertex_index_data->at(i).z - 1;

			unsigned int index = 0;
			bool found = getSimilarVertexIndex(
				in_vertices->at(vertex_index), in_normals->at(normal_index), in_textures->at(texture_index),
				vertices_, normals_, textures_, index
			);

			if (found) { // A similar vertex is already in the VBO, use it instead !
				indices_.push_back(index);
			}
			else { // If not, it needs to be added in the output data.
				vertices_.push_back(in_vertices->at(vertex_index));
				normals_.push_back(in_normals->at(normal_index));
				textures_.push_back(in_textures->at(texture_index));
				indices_.push_back(vertices_.size() - 1);
			}
		}
	}
}


