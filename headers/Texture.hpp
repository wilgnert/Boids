#ifndef TEXTURE_H
#define TEXTURE_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <SOIL2/SOIL2.h>
#include <iostream>

class Texture
{
private:

public:
    int width, height;
    GLuint id;
    std::string type;
    std::string path;

    Texture() {}
    Texture(const char * image_path, std::string type);
    ~Texture();

    inline GLuint getId() const { return this->id; }

    void bind();
    void unbind();
};

#endif // TEXTURE_H