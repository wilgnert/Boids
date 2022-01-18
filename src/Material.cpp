#include "../headers/Material.hpp"

Material::Material(
    glm::vec3 ambient,
    glm::vec3 diffuse,
    glm::vec3 specular,
    GLint diffuse_tex,
    GLint specular_tex,
    float shininess
) : ambient(ambient), diffuse(diffuse), specular(specular), diffuse_tex(diffuse_tex), specular_tex(specular_tex), shininess(shininess) {}

void Material::sendToShader(Shader& program) {
    program.setVec3("material.ambient", glm::vec3(this->ambient));
    program.setVec3("material.diffuse", glm::vec3(this->diffuse));
    program.setVec3("material.specular", glm::vec3(this->specular));
    program.setInt("material.texture_diffuse1", this->diffuse_tex);
    program.setInt("material.texture_specular1", this->specular_tex);
    program.setFloat("material.shininess", this->shininess);
}


Material::~Material() {}
