#ifndef OBJ_LOADER_H
#define OBJ_LOADER_H

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>
#include <algorithm>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Vertex.hpp"

static std::vector<Vertex> loadOBJ(const char* path) {
    // Vertex components
    std::vector<glm::fvec3> positions;
    std::vector<glm::fvec2> tex_coord;
    std::vector<glm::fvec3> normals;

    // Faces
    std::vector<GLint> position_indices;
    std::vector<GLint> normal_indices;
    std::vector<GLint> tex_coord_indices;

    // Vertex Array
    std::vector<Vertex> vertices;

    std::stringstream ss;
    std::ifstream in_file(path);
    std::string line("");

    if (!in_file.is_open()) {
        throw "ERROR::OBJ_LOADER::could_not_open_file";
    }

    while (std::getline(in_file, line))
    {
        ss.clear();
        std::string prefix = "";
        glm::fvec3 temp_vec3;
        glm::fvec2 temp_vec2;
        GLint temp_GLint;
        
        ss.str(line);
        ss >> prefix;

        if (prefix == "#") {
            continue;
        }
        else if (prefix == "o") {
            continue;
        }
        else if (prefix == "usemtl") {
            continue;
        }
        else if (prefix == "s") {
            continue;
        }
        else if (prefix == "v") { // Vertex position
            ss >> temp_vec3.x >> temp_vec3.y >> temp_vec3.z;
            positions.push_back(temp_vec3);
        }
        else if (prefix == "vt") { // Vertex texture coodinates
            ss >> temp_vec2.x >> temp_vec2.y;
            tex_coord.push_back(temp_vec2);
        }
        else if (prefix == "vn") { // Vertex normal
            ss >> temp_vec3.x >> temp_vec3.y >> temp_vec3.z;
            normals.push_back(temp_vec3);
        }
        else if (prefix == "f") {
            enum {
                POSITION = 1,
                TEX_COORD,
                NORMAL
            } counter = POSITION;
            while (ss >> temp_GLint)
            {
                if (counter == POSITION) {
                    position_indices.push_back(temp_GLint);
                    counter = TEX_COORD;
                }
                else if (counter == TEX_COORD) {
                    tex_coord_indices.push_back(temp_GLint);
                    counter = NORMAL;
                }
                else {
                    normal_indices.push_back(temp_GLint);
                    counter = POSITION;
                }

                ss.ignore(1, ss.peek());
            }
        }
        

        // // DEBUG
    }

    vertices.resize(position_indices.size(), Vertex());

    for (size_t i = 0; i < vertices.size(); i++)
    {
        vertices[i].position = positions[position_indices[i] - 1];
        vertices[i].texture_coord = tex_coord[tex_coord_indices[i] - 1];
        vertices[i].normal = normals[normal_indices[i] - 1];
    }
    

    // Load success
    return vertices;
}

#endif // OBJ_LOADER_H