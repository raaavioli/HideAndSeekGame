#pragma once

#include <vector>

#include "glad/glad.h"
#include "glm/glm.hpp"

#include "Engine/Core.h"
#include "Engine/Log.h"

#include "Engine/Objects/Camera.h"

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

		void BindViewProjectionMatrices(Camera *camera);

		//void BindEntityWorldMatrix(Entity *entity);

		void BindLightSource(glm::vec3 *light);

		void BindModelTexture(GLuint texture_id, unsigned int active_texture);

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

		void bindMatrix(GLuint uniform_id, glm::mat4 matrix);
		void bindVector3(GLuint uniform_id, glm::vec3 vector);
		void bindTexture2D(GLuint uniform_id, unsigned int active_texture);

		static ShaderProgram *s_ShaderProgram;
	};

}