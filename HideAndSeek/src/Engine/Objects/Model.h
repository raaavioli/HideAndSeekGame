#pragma once
#include <map>

#include "glm/glm.hpp"

#include <Engine/Core.h>
#include "VAO.h"
#include "VBO.h"

namespace Engine {

	class ENGINE_API Model
	{
	public:
		Model(VAO vao, std::map<VertexAttrib, VBO*> buffers, glm::vec3 min_pos, glm::vec3 max_pos)
			: m_VAO(vao), m_BufferMap(buffers),
			m_MaxPosition(max_pos), m_MinPosition(min_pos) {};
		~Model();
		void DrawModel();
		void DrawOutline();

		inline glm::vec3 &GetMinPos() { return m_MinPosition; }
		inline glm::vec3 &GetMaxPos() { return m_MaxPosition; }

	protected:
	private:
		VAO m_VAO;
		std::map<VertexAttrib, VBO*> m_BufferMap;
		glm::vec3 m_MaxPosition;
		glm::vec3 m_MinPosition;

		void DrawCall();
	};

}
