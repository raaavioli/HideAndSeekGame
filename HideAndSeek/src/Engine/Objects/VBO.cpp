#include "VBO.h"

namespace Engine {

	VBO::VBO(const std::vector<float> *data, int data_size, VertexAttrib va)
		: m_DataSize(data_size), m_VertexAttribute(va)
	{
		m_BufferType = GL_ARRAY_BUFFER;
		m_DataCount = data->size();

		glGenBuffers(1, &m_ID);
		glBindBuffer(m_BufferType, m_ID);
		glBufferData(m_BufferType, data->size() * sizeof(float), &data->at(0), GL_STATIC_DRAW);
	}

	VBO::VBO(const std::vector<glm::vec3> *data, VertexAttrib va) 
		: m_DataSize(3), m_VertexAttribute(va)
	{
		m_BufferType = GL_ARRAY_BUFFER;
		m_DataCount = data->size();

		glGenBuffers(1, &m_ID);
		glBindBuffer(m_BufferType, m_ID);
		glBufferData(m_BufferType, data->size() * sizeof(glm::vec3), &data->at(0), GL_STATIC_DRAW);
	}

	VBO::VBO(const std::vector<glm::vec2> *data, VertexAttrib va)
		: m_DataSize(2), m_VertexAttribute(va)
	{
		m_BufferType = GL_ARRAY_BUFFER;
		m_DataCount = data->size();

		glGenBuffers(1, &m_ID);
		glBindBuffer(m_BufferType, m_ID);
		glBufferData(m_BufferType, data->size() * sizeof(glm::vec2), &data->at(0), GL_STATIC_DRAW);
	}

	VBO::VBO(const std::vector<unsigned int> *data, VertexAttrib va)
		: m_DataSize(1), m_VertexAttribute(va)
	{
		m_BufferType = GL_ELEMENT_ARRAY_BUFFER;
		m_DataCount = data->size();

		glGenBuffers(1, &m_ID);
		glBindBuffer(m_BufferType, m_ID);
		glBufferData(m_BufferType, data->size() * sizeof(unsigned int), &data->at(0), GL_STATIC_DRAW);
	}

	Engine::VBO::~VBO()
	{
		glDeleteBuffers(1, &m_ID);
	}

	void Engine::VBO::Bind()
	{
		glBindBuffer(m_BufferType, m_ID);

		if (isIndexBuffer()) { return; }

		glEnableVertexAttribArray(m_VertexAttribute);
		glVertexAttribPointer(
			m_VertexAttribute,
			m_DataSize,
			GL_FLOAT,
			GL_FALSE,
			0,
			(void*)0
		);
	}

}

