#pragma once

#include <glad/glad.h>

#include "Engine/Core.h"

namespace Engine {
	class ENGINE_API VAO 
	{
	public:
		VAO();
		~VAO();

		inline void Bind() { glBindVertexArray(m_ID); }

	private:
		unsigned int m_ID;
	};
}