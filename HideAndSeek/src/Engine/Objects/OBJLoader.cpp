#include "OBJLoader.h"

#include "Engine/Log.h"

#pragma warning(disable:4996)

namespace Engine {
	OBJLoader* OBJLoader::s_Instance= nullptr;

	std::vector<VAO*> OBJLoader::loadedVAOs;
	std::vector<VBO*> OBJLoader::loadedVBOs;
	std::map<std::string, Model*> OBJLoader::modelCache;

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
		modelCache	= std::map<std::string, Model*>();

		vertices_	= std::vector<glm::vec3>();
		normals_	= std::vector<glm::vec3>();
		textures_	= std::vector<glm::vec2>();
		indices_	= std::vector<unsigned int>();
	}

	void OBJLoader::Shutdown() {
		for (VAO *vao : loadedVAOs) 
			delete vao;

		for (VBO *vbo : loadedVBOs) 
			delete vbo;

		for (auto &[key, value] : modelCache)
			delete value;
	}

	Model *OBJLoader::GetModel(const char* filename, bool normalize, bool centralize) {
		std::map<std::string, Model*>::const_iterator iter = modelCache.find(std::string(filename));

		if (iter != modelCache.end()) {
			return (*iter).second;
		} 
		else 
			return loadObjFile(filename, normalize, centralize);
	}

	Model *OBJLoader::loadObjFile(const char* filename, bool normalize, bool centralize) {
		std::string fn = "src/Application/objects/" + std::string(filename) + ".obj";
		FILE* file = fopen(fn.c_str(), "r");
		errno = 0;

		CORE_ASSERT((file != NULL), "File not found");
		CORE_INFO("Filename: '{0}' ", fn.c_str());

		std::vector<glm::uvec3> vertex_index_data;
		std::vector<glm::vec3> temp_vertices;
		std::vector<glm::vec3> temp_normals;
		std::vector<glm::vec2> temp_textures;

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
			
			for (int i = 0; i < vertices_.size(); i++) {
				if (centralize) {
					vertices_.at(i) -= v_Center;
				}
				if (normalize) {
					vertices_.at(i) /= max_dist;
				}

				AddMaxVertex(v_MaxTemp, vertices_.at(i));
				AddMinVertex(v_MinTemp, vertices_.at(i));
			}
		}


		

		VAO vao = VAO();
		std::map<VertexAttrib, VBO*> vbos;
		vbos[VERTEX] = new VBO(&vertices_, VertexAttrib::VERTEX);
		vbos[NORMAL] = new VBO(&normals_, VertexAttrib::NORMAL);
		vbos[TEXTURE] = new VBO(&textures_, VertexAttrib::TEXTURE);
		vbos[INDEX] = new VBO(&indices_, VertexAttrib::INDEX);

		if (normalize || centralize) {
			v_MaxCoords = v_MaxTemp;
			v_MinCoords = v_MinTemp;
		}

		Model* m = new Model(vao, vbos, v_MinCoords, v_MaxCoords);
		modelCache.insert(std::make_pair(std::string(filename), m));
		return m;
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


