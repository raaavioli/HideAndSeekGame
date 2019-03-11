#include "VAO.h"

namespace Engine {

	VAO::VAO() 
	{
		glGenVertexArrays(1, &m_ID);
		glBindVertexArray(m_ID);
	}

	VAO::~VAO() {
		glDeleteVertexArrays(1, &m_ID);
	}
}