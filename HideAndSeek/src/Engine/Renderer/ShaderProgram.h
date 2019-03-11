#pragma once

#include <vector>

#include "glad/glad.h"
#include "glm/glm.hpp"

#include "Engine/Core.h"
#include "Engine/Log.h"

#include "Engine/Objects/Camera.h"
#include "Engine/Objects/Entity.h"

namespace Engine {

	class ENGINE_API ShaderProgram
	{
		ShaderProgram() {};
	public:
		~ShaderProgram();

		static void Init(const char* vertex_file, const char* fragment_file);
		static void Shutdown();
		inline static ShaderProgram &GetShader() { return *s_ShaderProgram; }

		static void Bind();
		static void BindUniformLocations();
		static ShaderProgram& Get() { return *s_ShaderProgram; }
		static GLuint GetProgramId();

		static void BindViewProjectionMatrices(Camera *camera);
		static void BindEntityWorldMatrix(glm::mat4 *m_Transformation);
		static void BindLightSource(glm::vec3 *light);
		static void BindModelTexture(GLuint texture_id, unsigned int active_texture);
		static void BindVertexColor(glm::vec3 *color);

	private:
		static std::string read(const char* file_path);
		static GLuint compileShader(const std::string shaderCode, const char* file_path, GLuint shader_type);
		static void createProgram(const GLuint v_shader, const GLuint f_shader);

		static GLuint m_ProgramId;
		static GLuint m_ProjectionUniform;
		static GLuint m_ViewUniform;
		static GLuint m_ModelUniform;
		static GLuint m_LightPositionUniform;
		static GLuint m_ModelTextureUniform;
		static GLuint m_VertexColor;

		static void bindMatrix(GLuint uniform_id, glm::mat4 *matrix);
		static void bindVector3(GLuint uniform_id, glm::vec3 *vector);
		static void bindTexture2D(GLuint uniform_id, unsigned int active_texture);

		static ShaderProgram *s_ShaderProgram;
	};

}