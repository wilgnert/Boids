#ifndef SHADER_H
#define SHADER_H

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <glm/mat4x4.hpp>
#include <glm/gtc/type_ptr.hpp>

class Shader
{
public:
	// A ID do programa
	// Construtor que lê e constroi a shader 
	Shader(const int version_major, const int version_minor, const char* vertexPath, const char* fragmentPath, const char * geometryPath = "");
	// Função para usar este shader
	~Shader();
	void use() const;
	void unuse() const;

	GLuint id() const {
		return this->_id;
	}
	// Fuções de utilidade para settar valores para uniformes no programa do shader

	void setBool(const std::string& uniformName, bool value) const;
	void setInt(const std::string& uniformName, int value) const;
	void setFloat(const std::string& uniformName, float value) const;
	void setVec3(const std::string& uniformName, glm::vec3 value) const;
	void setVec4(const std::string& uniformName, glm::vec4& value) const;
	void setMat4(const std::string& uniformName, glm::mat4& value, GLboolean transpose) const;

private:
	GLuint _id;
	const int version_major;
	const int version_minor;

	std::string loadShaderSrc(const char *file_name);

	GLuint loadShader(GLenum type, const char *file_name);

	void linkProgram(GLuint vertex_shader, GLuint fragment_shader, GLuint geometry_shader);
};

#endif // SHADER_H