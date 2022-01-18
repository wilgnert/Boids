#ifndef MATERIAL_H
#define MATERIAL_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <glm/mat4x4.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "shader.hpp"

class Material
{
private:
    glm::vec3 ambient;
    glm::vec3 diffuse;
    glm::vec3 specular;
    float shininess;
    GLint diffuse_tex;
    GLint specular_tex;
    /* data */
public:
    Material(
        glm::vec3 ambient,
        glm::vec3 diffuse,
        glm::vec3 specular,
        GLint diffuse_tex,
        GLint specular_tex,
        float shininess = 0.5f
    );
    ~Material();

    void sendToShader(Shader& program);
};

#endif // MATERIAL_H