#include "Model.h"

namespace Engine {

	Model::Model(VAO *vao, VBO *vertices, VBO *normals, VBO *textures, VBO *indices)
		: m_VAO(vao), m_Vertices(vertices), m_Normals(normals),
		m_TextureCoords(textures), m_IndexBuffer(indices)
	{

	}

	Model::~Model()
	{

	}
	
	void Model::DrawModel() 
	{
		//Set to assert m_Normals and m_TextureCoords when fully implemented
		CORE_ASSERT(m_VAO && m_Vertices && m_IndexBuffer, 
			"A buffer was not successfully initialized. Buffers must not be null.\nTry DrawOutline if Model does not have textures/normals"
		);
		
		// Bind all buffers
		m_VAO			-> Bind();
		m_Vertices		-> Bind(VertexAttrib::VERTEX);
		//m_Normals		-> Bind(VertexAttrib::NORMAL);
		//m_TextureCoords	-> Bind(VertexAttrib::TEXTURE);
		m_IndexBuffer	-> Bind(VertexAttrib::INDEX);

		//Draw lines/non filled triangles
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		// Draw triangles 
		glDrawElements(
			GL_TRIANGLES,				// mode
			m_IndexBuffer->GetCount(),	// count
			GL_UNSIGNED_INT,			// type
			(void*)0					// element array buffer offset
		);
	};

	void Model::DrawOutline()
	{

		CORE_ASSERT(m_VAO && m_Vertices && m_IndexBuffer,
			"A buffer was not successfully initialized. Buffers must not be null.\nTry DrawOutline if Model does not have textures/normals"
		);
		// Bind buffers
		m_VAO->Bind();
		m_Vertices->Bind(VertexAttrib::VERTEX);
		m_IndexBuffer->Bind(VertexAttrib::INDEX);

		//Draw lines/non filled triangles
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		// Draw triangles 
		glDrawElements(
			GL_TRIANGLES,				// mode
			m_IndexBuffer->GetCount(),	// count
			GL_UNSIGNED_INT,			// type
			(void*)0					// element array buffer offset
		);
	};


}
