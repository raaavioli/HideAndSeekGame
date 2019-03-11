#include "ShaderProgram.h"

#include <fstream>
#include <sstream>

namespace Engine {

	ShaderProgram* ShaderProgram::s_ShaderProgram	= nullptr;
	GLuint ShaderProgram::m_ProgramId				= 0;
	GLuint ShaderProgram::m_ProjectionUniform		= 0;
	GLuint ShaderProgram::m_ViewUniform 			= 0;
	GLuint ShaderProgram::m_ModelUniform 			= 0;
	GLuint ShaderProgram::m_ModelTextureUniform		= 0;
	GLuint ShaderProgram::m_LightPositionUniform	= 0; 

	ShaderProgram::~ShaderProgram() {
	}

	void ShaderProgram::Init(const char* vertex_file, const char* fragment_file) 
	{
		CORE_ASSERT(!s_ShaderProgram, "ShaderProgram is already Initialized");
		s_ShaderProgram = new ShaderProgram();

		std::string v_code = read(vertex_file);
		std::string f_code = read(fragment_file);

		GLuint v_shader = compileShader(v_code, vertex_file, GL_VERTEX_SHADER);
		GLuint f_shader = compileShader(f_code, fragment_file, GL_FRAGMENT_SHADER);

		createProgram(v_shader, f_shader);

		glDetachShader(m_ProgramId, v_shader);
		glDetachShader(m_ProgramId, f_shader);
		glDeleteShader(v_shader);
		glDeleteShader(f_shader);

		BindUniformLocations();
		Bind();
	}

	void ShaderProgram::Shutdown()
	{
		glDeleteProgram(m_ProgramId);
		delete s_ShaderProgram;
	}

	GLuint ShaderProgram::GetProgramId() 
	{
		return m_ProgramId;
	}

	void ShaderProgram::Bind() 
	{
		glUseProgram(m_ProgramId);
	}

	void ShaderProgram::BindUniformLocations()
	{
		m_ProjectionUniform		= glGetUniformLocation(m_ProgramId, "m_projection");
		m_ViewUniform			= glGetUniformLocation(m_ProgramId, "m_view");
		m_ModelUniform			= glGetUniformLocation(m_ProgramId, "m_model");
		m_LightPositionUniform	= glGetUniformLocation(m_ProgramId, "light_position");
		m_ModelTextureUniform	= glGetUniformLocation(m_ProgramId, "model_texture");
	}

	std::string ShaderProgram::read(const char* file_path) 
	{
		std::string shaderCode;
		std::ifstream shaderStream(file_path, std::ios::in);
		if (shaderStream.is_open()) {
			std::stringstream sstr;
			sstr << shaderStream.rdbuf();
			shaderCode = sstr.str();
			shaderStream.close();
		}
		else {
			CORE_ERROR("Impossible to open \"{0}\". Wrong directory?", std::string(file_path));
			getchar();
			return 0;
		}
		return shaderCode;
	}

	GLuint ShaderProgram::compileShader(const std::string shaderCode, const char* file_path, GLuint shader_type) 
	{
		GLuint shader_id = glCreateShader(shader_type);
		GLint Result = GL_FALSE;
		int InfoLogLength;

		// Compile Vertex Shader
		CORE_INFO("Compiling shader: {0}", std::string(file_path));

		char const * sourcePointer = shaderCode.c_str();
		glShaderSource(shader_id, 1, &sourcePointer, NULL);
		glCompileShader(shader_id);

		// Check Vertex Shader
		glGetShaderiv(shader_id, GL_COMPILE_STATUS, &Result);
		glGetShaderiv(shader_id, GL_INFO_LOG_LENGTH, &InfoLogLength);

		if (InfoLogLength > 0) {
			std::vector<char> msg(InfoLogLength + 1);
			glGetShaderInfoLog(shader_id, InfoLogLength, NULL, &msg[0]);
			std::string error(msg.begin(), msg.end());
			CORE_ERROR("{0}", error);
		}

		return shader_id;
	}

	void ShaderProgram::createProgram(const GLuint v_shader_id, const GLuint f_shader_id) 
	{
		GLint Result = GL_FALSE;
		int InfoLogLength;

		// Link the program
		CORE_INFO("Linking shader-program");
		m_ProgramId = glCreateProgram();
		glAttachShader(m_ProgramId, v_shader_id);
		glAttachShader(m_ProgramId, f_shader_id);
		glLinkProgram(m_ProgramId);

		// Check the program
		glGetProgramiv(m_ProgramId, GL_LINK_STATUS, &Result);
		glGetProgramiv(m_ProgramId, GL_INFO_LOG_LENGTH, &InfoLogLength);
		if (InfoLogLength > 0) {
			std::vector<char> msg(InfoLogLength + 1);
			glGetShaderInfoLog(m_ProgramId, InfoLogLength, NULL, &msg[0]);
			std::string error(msg.begin(), msg.end());
			CORE_ERROR("{0}", error);
		}
	}


	void ShaderProgram::bindMatrix(GLuint uniform_id, glm::mat4 matrix) 
	{
		glUniformMatrix4fv(
			uniform_id,
			1,
			GL_FALSE,
			&matrix[0][0]
		);
	}

	void ShaderProgram::bindVector3(GLuint uniform_id, glm::vec3 vector) 
	{
		glUniform3fv(uniform_id, 1, (GLfloat*)&vector);
	}

	void ShaderProgram::bindTexture2D(GLuint uniform_id, unsigned int active_texture) 
	{
		glUniform1i(uniform_id, active_texture);
	}

	void ShaderProgram::BindViewProjectionMatrices(Camera *camera) {
		bindMatrix(m_ViewUniform, camera->GetViewMatrix());
		bindMatrix(m_ProjectionUniform, camera->GetProjectionMatrix());
	}

	/*void ShaderProgram::BindEntityWorldMatrix(Entity *entity) {
		bindMatrix(m_ModelUniform, entity->getWorldTransformation());
	}*/

	void ShaderProgram::BindLightSource(glm::vec3 *light) 
	{
		glUniform3fv(m_LightPositionUniform, 1, (GLfloat*)light);
	}

	void ShaderProgram::BindModelTexture(GLuint texture_id, unsigned int active_texture) 
	{
		glBindTexture(GL_TEXTURE_2D, texture_id);
		glUniform1i(m_ModelTextureUniform, active_texture);
	}
}