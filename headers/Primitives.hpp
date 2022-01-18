#ifndef PRIMITIVES_H
#define PRIMITIVES_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <vector>
#include "Vertex.hpp"

class Primitive
{
private:
    std::vector<Vertex> vertices;
    std::vector<GLuint> indices;

public:
    Primitive() {}
    virtual ~Primitive() {}

    // Functions
    void set (const Vertex* vertices, const unsigned num_vertices, const GLuint* indices, const unsigned num_indices) {
        for (size_t i = 0; i < num_vertices; i++) { this->vertices.push_back(vertices[i]); }
        for (size_t i = 0; i < num_indices; i++) { this->indices.push_back(indices[i]); }
    }
    inline std::vector<Vertex> getVertices() { return this->vertices; }
    inline std::vector<GLuint> getIndices() { return this->indices; }
    inline const unsigned getNumVertices()  { return vertices.size(); }
    inline const unsigned getNumIndices()  { return indices.size(); }
};

class Triangle : public Primitive
{
public:
    Triangle() 
        : Primitive() 
    {
        Vertex vertices[] =
		{
            positions[0], normals[0],  glm::vec2(1.f, 0.f),
            positions[1], normals[1],  glm::vec2(0.f, 0.f),    
            positions[2], normals[2],  glm::vec2(0.f, 0.f)
        };
        unsigned nrOfVertices = sizeof(vertices) / sizeof(Vertex);

		GLuint indices[] =
		{
			0, 1, 2,	//Triangle 1
		};
		unsigned nrOfIndices = sizeof(indices) / sizeof(GLuint);

		this->set(vertices, nrOfVertices, indices, nrOfIndices);
    }
private:
    glm::vec3 positions[3] = {
        glm::vec3(0.f, 0.5f, 0.f),
        glm::vec3(-0.5f, -0.5f, 0.5f),
        glm::vec3(0.5f, -0.5f, 0.5f)
    };
    glm::vec3 normals[3] = {
        glm::normalize(glm::vec3(0.f, -0.5f, 0.f)),
        glm::normalize(glm::vec3(0.5f, 0.5f, -0.5f)),
        glm::normalize(glm::vec3(-0.5f, 0.5f, -0.5f))
    };
};

class Quad : public Primitive
{
public:
    Quad() : Primitive() {
        Vertex vertices[] =
		{
            positions[0], normals[0], glm::vec2(1.f, 0.f),
            positions[1], normals[1], glm::vec2(0.f, 0.f),    
            positions[2], normals[2], glm::vec2(0.f, 0.f),
            positions[3], normals[3], glm::vec2(1.f, 1.f)   
        };
        unsigned nrOfVertices = sizeof(vertices) / sizeof(Vertex);

		GLuint indices[] =
		{
			0, 1, 2,	//Triangle 1
			0, 2, 3		//Triangle 2
		};
		unsigned nrOfIndices = sizeof(indices) / sizeof(GLuint);

		this->set(vertices, nrOfVertices, indices, nrOfIndices);
    }
private:
    glm::vec3 positions[4] = {
        glm::vec3(-0.5f, 0.5f, 0.5f),
        glm::vec3(-0.5f, -0.5f, 0.5f),
        glm::vec3(0.5f, 0.5f, 0.5f),
        glm::vec3(0.5f, -0.5f, 0.5f)
    };
    glm::vec3 normals[4] = {
        glm::normalize(glm::vec3(0.5f, -0.5f, -0.5f)),
        glm::normalize(glm::vec3(0.5f, 0.5f, -0.5f)),
        glm::normalize(glm::vec3(-0.5f, -0.5f, -0.5f)),
        glm::normalize(glm::vec3(-0.5f, 0.5f, -0.5f))
    };
};

class Cube : public Primitive 
{
private:
    glm::vec3 positions[8] = {
        glm::vec3(-0.5f, 0.5f, 0.5f),
        glm::vec3(-0.5f, -0.5f, 0.5f),
        glm::vec3(0.5f, 0.5f, 0.5f),
        glm::vec3(0.5f, -0.5f, 0.5f),

        glm::vec3(-0.5f, 0.5f, -0.5f),
        glm::vec3(-0.5f, -0.5f, -0.5f), 
        glm::vec3(0.5f, 0.5f, -0.5f),
        glm::vec3(0.5f, -0.5f, -0.5f),
    };
    glm::vec3 normals[8] = {
        glm::normalize(glm::vec3(0.5f, -0.5f, -0.5f)),
        glm::normalize(glm::vec3(0.5f, 0.5f, -0.5f)),
        glm::normalize(glm::vec3(-0.5f, -0.5f, -0.5f)),
        glm::normalize(glm::vec3(-0.5f, 0.5f, -0.5f)),

        glm::normalize(glm::vec3(0.5f, -0.5f, 0.5f)),
        glm::normalize(glm::vec3(0.5f, 0.5f, 0.5f)),
        glm::normalize(glm::vec3(-0.5f, -0.5f, 0.5f)),
        glm::normalize(glm::vec3(-0.5f, 0.5f, 0.5f))
    };
    glm::vec2 uvs[4] = {
        glm::vec2(1.f, 1.f),
        glm::vec2(0.f, 1.f),
        glm::vec2(1.f, 0.f),
        glm::vec2(0.f, 0.f)};

public:
    Cube() 
    : Primitive() 
    {
        Vertex vertices[] {
            // FRONT
        /* 0 */ positions[0], normals[0],  uvs[1],
        /* 1 */ positions[1], normals[1],  uvs[3],
        /* 2 */ positions[2], normals[2],  uvs[0],
        /* 3 */ positions[3], normals[3],  uvs[2],

            // BACK
        /* 4 */ positions[4], normals[4],  uvs[0],
        /* 5 */ positions[5], normals[5],  uvs[2],
        /* 6 */ positions[6], normals[6],  uvs[1],
        /* 7 */ positions[7], normals[7],  uvs[3],

            // BOTTOM
        /* 8 */ positions[1], normals[1],  uvs[1],
        /* 9 */ positions[5], normals[5],  uvs[3],
        /* 10 */positions[7], normals[7],  uvs[2],
        /* 11 */positions[3], normals[3],  uvs[0],

            // TOP
        /* 12 */positions[0], normals[0],  uvs[3],
        /* 13 */positions[4], normals[4],  uvs[1],
        /* 15 */positions[6], normals[6],  uvs[0],
        /* 14 */positions[2], normals[2],  uvs[2],
            
            // LEFT
        /* 16 */positions[0], normals[0],  uvs[0],
        /* 19 */positions[4], normals[4],  uvs[1],
        /* 17 */positions[5], normals[5],  uvs[3],
        /* 18 */positions[1], normals[1],  uvs[2],

            // RIGHT
        /* 20 */positions[3], normals[3],  uvs[3],
        /* 21 */positions[7], normals[7],  uvs[2],
        /* 22 */positions[6], normals[6],  uvs[0],
        /* 23 */positions[2], normals[2],  uvs[1]

        };
        unsigned num_vertices = sizeof(vertices) / sizeof(Vertex);

        unsigned indices[] = {
            0, 1, 2,
            2, 1, 3,
            
            4, 6, 5,
            5, 6, 7,

            8, 9, 10,
            8, 10, 11,

            12, 14, 13,
            12, 15, 14,

            16, 17, 18,
            16, 18, 19,

            20, 21, 22,
            20, 22, 23
        };
        unsigned num_indices = sizeof(indices) / sizeof(GLuint);

        this->set(vertices, num_vertices, indices, num_indices);
    }
    ~Cube(){};
};

#endif // PRIMITIVES_H