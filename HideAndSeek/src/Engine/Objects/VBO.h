#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>

#include "Engine/Core.h"
#include "Engine/Log.h"

namespace Engine {

	enum VertexAttrib {
		INDEX = -1,
		VERTEX,
		NORMAL,
		TEXTURE,
	};

	class ENGINE_API VBO
	{
	public:
		VBO(const std::vector<float> *data, int data_size, VertexAttrib va);
		VBO(const std::vector<glm::vec3> *data, VertexAttrib va);
		VBO(const std::vector<glm::vec2> *data, VertexAttrib va);
		VBO(const std::vector<unsigned int> *data, VertexAttrib va);

		
		~VBO();

		void Bind();
		inline int GetCount() { return m_DataCount; }
		inline int GetID() { return m_ID; }
		inline GLuint GetBType() { return m_BufferType; }

	private:
		unsigned int m_ID;
		int m_DataCount;
		int m_DataSize;
		GLuint m_BufferType;
		VertexAttrib m_VertexAttribute;

		inline bool isIndexBuffer() { return m_BufferType == GL_ELEMENT_ARRAY_BUFFER; }
	};
}