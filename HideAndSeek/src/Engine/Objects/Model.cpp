#include "Model.h"

namespace Engine {

	Model::~Model()
	{

	}
	
	void Model::DrawModel() 
	{
		// Bind buffers
		m_VAO.Bind();
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
		// Bind buffers
		m_VAO.Bind();
		for (auto &buffer : m_BufferMap) {
			buffer.second->Bind();
		}
		//Draw lines/non filled triangles
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		// Draw triangles 
		DrawCall();
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
