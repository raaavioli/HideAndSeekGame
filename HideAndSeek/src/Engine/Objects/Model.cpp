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
	
	void Model::Draw() 
	{
		// Bind all buffers
		//m_VAO			-> Bind();
		m_Vertices		-> Bind(VertexAttrib::VERTEX);
		//m_Normals		-> Bind(VertexAttrib::NORMAL);
		//m_TextureCoords	-> Bind(VertexAttrib::TEXTURE);
		m_IndexBuffer	-> Bind(VertexAttrib::INDEX);

		// Draw triangles 
		glDrawElements(
			GL_TRIANGLES,				// mode
			m_IndexBuffer->GetCount(),	// count
			GL_UNSIGNED_INT,			// type
			(void*)0					// element array buffer offset
		);
	};


}
