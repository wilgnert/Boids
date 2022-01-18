#ifndef MESH_H
#define MESH_H

#include <iostream>
#include <vector>

#include "Material.hpp"
#include "shader.hpp"
#include "Texture.hpp"
#include "Primitives.hpp"

#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>

class Mesh
{
private:
    /* data */
    std::vector<Vertex>       vertices;
    std::vector<unsigned int> indices;
    std::vector<Texture>      textures;

    GLuint VAO, VBO, EBO;

    glm::mat4 model_matrix;
    glm::vec3 origin;
    glm::vec3 position;
    glm::vec3 rotation;
    glm::vec3 _scale;

    void setup()
    {
        glGenVertexArrays(1, &this->VAO);
        glGenBuffers(1, &this->VBO);
        glGenBuffers(1, &EBO);

        glBindVertexArray(this->VAO);
        glBindBuffer(GL_ARRAY_BUFFER, this->VBO);
        
        glBufferData(GL_ARRAY_BUFFER, this->vertices.size() * sizeof(Vertex), &this->vertices[0], GL_STATIC_DRAW);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, this->indices.size() * sizeof(unsigned), &this->indices[0], GL_STATIC_DRAW);

        // Position
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)offsetof(Vertex, position));
        // Texture Coordinates
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)offsetof(Vertex, texture_coord));
        // Normal
        glEnableVertexAttribArray(2);
        glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)offsetof(Vertex, normal));

        glBindVertexArray(0);
    }

    void updateUniforms(Shader *shader)
    {
        shader->setMat4("model_matrix", model_matrix, GL_FALSE);
    }

    void updateModelMatrix()
    {
        model_matrix = glm::mat4(1.f);
        
        glm::quat quater(glm::vec3(glm::radians(this->rotation.x), glm::radians(this->rotation.y), glm::radians(this->rotation.z)));
        
        glm::mat4 rotation_matrix = glm::toMat4(quater);
        
        model_matrix = glm::translate(model_matrix, origin) * rotation_matrix * glm::translate(model_matrix, position - origin) * glm::scale(model_matrix, _scale);
        
        
        // model_matrix = glm::translate(model_matrix, origin);
        // model_matrix = glm::rotate(model_matrix, rotation.x, glm::vec3(1.f, 0.f, 0.f));
        // model_matrix = glm::rotate(model_matrix, rotation.y, glm::vec3(0.f, 1.f, 0.f));
        // model_matrix = glm::rotate(model_matrix, rotation.z, glm::vec3(0.f, 0.f, 1.f));
        // model_matrix = glm::translate(model_matrix, position - origin);
        // model_matrix = glm::scale(model_matrix, _scale);
    }

public:
    
    Mesh(const Mesh &mesh)
    {
        *this = mesh;

        this->setup();
        this->updateModelMatrix();
    }
    
    Mesh(std::vector<Vertex> vertices, std::vector<unsigned> indices, std::vector<Texture> textures)
     : origin(0.f), position(0.f), rotation(0.f), _scale(1.f)
    {
        this->vertices = vertices;
        this->indices = indices;
        this->textures = textures;

        setup();
    }

    ~Mesh()
    {
        glDeleteVertexArrays(1, &VAO);
        glDeleteBuffers(1, &VBO);
        glDeleteBuffers(1, &EBO);
    }

    void update()
    {
        updateModelMatrix();
    }
    void render(Shader *shader)
    {
        updateUniforms(shader);

        unsigned int diffuseNr = 1;
        unsigned int specularNr = 1;
        for(unsigned int i = 0; i < textures.size(); i++)
        {
            glActiveTexture(GL_TEXTURE0 + i); // activate proper texture unit before binding
            // retrieve texture number (the N in diffuse_textureN)
            std::string number;
            std::string name = textures[i].type;
            if(name == "texture_diffuse")
                number = std::to_string(diffuseNr++);
            else if(name == "texture_specular")
                number = std::to_string(specularNr++);

            shader->setFloat(("material." + name + number).c_str(), i);
            textures[i].bind();
        }
        glActiveTexture(GL_TEXTURE0);

        glBindVertexArray(VAO);
        shader->use();
        if (this->indices.size() == 0)
            glDrawArrays(GL_TRIANGLES, 0, this->vertices.size());
        else
            glDrawElements(GL_TRIANGLES, this->indices.size(), GL_UNSIGNED_INT, 0);

        glBindVertexArray(0);
        glUseProgram(0);
        glActiveTexture(0);
        glBindTexture(GL_TEXTURE_2D, 0);
    }

    // Geters
    glm::vec3 getPosition() { return this->position; }
    glm::vec3 getOrigin() { return this->origin; }
    glm::vec3 getRotation() { return this->rotation; }
    glm::vec3 getScale() { return this->_scale; }

    // Setters
    void setPosition(glm::vec3 pos) { this->position = pos; }
    void setOrigin(glm::vec3 org) { this->origin = org; }
    void setRotation(glm::vec3 rot) { this->rotation = rot; }
    void setScale(glm::vec3 scl) { this->_scale = scl; }

    // Funcs
    void scale(glm::vec3 scl) { this->_scale += scl; }
    void move(glm::vec3 pos) { this->position += pos; }
    void rotate(glm::vec3 rot) { this->rotation += rot; }
};

#endif // MESH_H