#include "Model.h"

namespace Engine {

	Model::~Model()
	{

	}
	
	void Model::DrawModel() 
	{
		CORE_ASSERT(m_VAO, "VAO was not successfully initialized. nullpointer. \n");
		// Bind buffers
		m_VAO->Bind();
		for (auto &buffer : m_BufferMap) {
			buffer.second->Bind();
		}

		//Draw lines/non filled triangles
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		// Draw triangles 
		DrawCall();
	};

	void Model::DrawOutline()
	{
		CORE_ASSERT(m_VAO, "VAO was not successfully initialized. nullpointer. \n");
		// Bind buffers
		m_VAO->Bind();
		for (auto &buffer : m_BufferMap) {
			buffer.second->Bind();
		}
		//Draw lines/non filled triangles
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		// Draw triangles 
		DrawCall();
	}

	glm::vec3 Model::getModelCenter()
	{
		return (m_MaxPosition + m_MinPosition) / glm::vec3(2.0);
	}

	void Model::DrawCall() {
		glDrawElements(
			GL_TRIANGLES,									// Drawing mode (GL_TRIANGLES, GL_LINES ...)
			m_BufferMap[VertexAttrib::INDEX]->GetCount(),	// Index count (amount of vertex indices to process)
			GL_UNSIGNED_INT,								// data type
			(void*)0										// element array buffer offset
		);
	}
}
