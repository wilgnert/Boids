#include "../headers/shader.hpp"
std::string Shader::loadShaderSrc(const char *file_name){
	std::string src_code;
	std::ifstream shader_file;

	shader_file.exceptions(std::ifstream::failbit | std::ifstream::badbit);

	try {
		std::stringstream shader_stream;

		shader_file.open(file_name);
		shader_stream << shader_file.rdbuf();
		shader_file.close();
		src_code = shader_stream.str();
	}
	catch (const std::ifstream::failure e)
	{
		std::cout << "ERROR::SHADER::FILE_NOT_SUCCESSFULLY_READ: " << file_name << std::endl;
	}

	std::string version = std::to_string(version_major) + std::to_string(version_minor) + "0";
	src_code.replace(src_code.find("#version"), 12, ("#version " + version));

	return src_code;
}

GLuint Shader::loadShader(GLenum type, const char *file_name) {
	std::string code = loadShaderSrc(file_name);
	const char *src = code.c_str();

	unsigned id;
	int success;
	char info_log[512];

	id = glCreateShader(type);
	glShaderSource(id, 1, &src, NULL);
	glCompileShader(id);
	glGetShaderiv(id, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(id, 512, NULL, info_log);
		std::cout << "ERROR::SHADER::";
		switch (type)
		{
		case GL_VERTEX_SHADER:
			std::cout << "VERTEX";
			break;
		case GL_FRAGMENT_SHADER:
			std::cout << "FRAGMENT";
			break;
		case GL_GEOMETRY_SHADER:
			std::cout << "GEOMETRY";
			break;
		default:
			break;
		}
		std::cout << "::COMPILATION_FAILED\n" << info_log << std::endl;
	}

	return id;
}

Shader::~Shader() {
	glDeleteProgram(this->id());
}

Shader::Shader(const int version_major, const int version_minor, 
				const char* vertexPath, const char* fragmentPath,  const char * geometryPath)
 				: version_major(version_major), version_minor(version_minor)
{
	unsigned int vert = 0, frag = 0, geom = 0;
	vert = loadShader(GL_VERTEX_SHADER, vertexPath);
	frag = loadShader(GL_FRAGMENT_SHADER, fragmentPath);
	if (geometryPath[0] != 0) geom = loadShader(GL_GEOMETRY_SHADER, geometryPath);

	// Liga��o do shader de vert�ce e o de fragmento em um programa
	this->_id = glCreateProgram();
	glAttachShader(this->_id, vert);
	glAttachShader(this->_id, frag);
	if (geom) glAttachShader(this->_id, geom);
	glLinkProgram(this->_id);

	int success;
	char infoLog[512];

	glGetProgramiv(this->_id, GL_LINK_STATUS, &success);
	if (!success)
	{
		glGetProgramInfoLog(this->_id, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
	}

	glDeleteShader(vert);
	glDeleteShader(frag);
	if (geom) glDeleteShader(geom);
	this->unuse();
}

void Shader::use() const {
	glUseProgram(this->_id);
}

void Shader::unuse() const {
	glUseProgram(0);
}

void Shader::setBool(const std::string& uniformName, bool value) const
{
	use();
	glUniform1i(glGetUniformLocation(this->_id, uniformName.c_str()), (int)value);
	unuse();
}

void Shader::setInt(const std::string& uniformName, int value) const
{
	use();
	glUniform1i(glGetUniformLocation(this->_id, uniformName.c_str()), value);	
	unuse();
}

void Shader::setFloat(const std::string& uniformName, float value) const
{
	use();
	glUniform1f(glGetUniformLocation(this->_id, uniformName.c_str()), value);
	unuse();
}

void Shader::setVec3(const std::string& uniformName, glm::vec3 value) const
{
	use();
	glUniform3fv(glGetUniformLocation(this->_id, uniformName.c_str()), 1, glm::value_ptr(value));
	unuse();
}

void Shader::setVec4(const std::string& uniformName, glm::vec4& value) const
{
	use();
	glUniform4f(glGetUniformLocation(this->_id, uniformName.c_str()), value.x, value.y, value.z, value.w);
	unuse();
}

void Shader::setMat4(const std::string& uniformName, glm::mat4& value, GLboolean transpose) const
{	
	use();
	glUniformMatrix4fv(glGetUniformLocation(this->_id, uniformName.c_str()), 1, transpose, glm::value_ptr(value));
	unuse();
}
