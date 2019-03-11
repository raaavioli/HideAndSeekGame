#pragma once

#include <Engine/Core.h>
#include "VAO.h"
#include "VBO.h"

namespace Engine {

	class ENGINE_API Model
	{
	public:
		Model(VAO *vao, VBO *vertices, VBO *normals, VBO *textures, VBO *indices);
		~Model();
		void Draw();

	protected:
	private:
		VAO *m_VAO;
		VBO *m_Vertices;
		VBO *m_Normals;
		VBO *m_TextureCoords;
		VBO *m_IndexBuffer;
	};

}
