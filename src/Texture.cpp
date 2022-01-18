#include "../headers/Texture.hpp"

Texture::~Texture() {
    glDeleteTextures(1, &id);
}

Texture::Texture(const char * file_path, std::string type){
    glGenTextures(1, &id);
    glBindTexture(GL_TEXTURE_2D, id);

    int nrComponents;
    unsigned char *image = SOIL_load_image(file_path, &width, &height, &nrComponents, 0);

    if (image) {

        GLenum format;
        if (nrComponents == 1)
            format = GL_RED;
        else if (nrComponents == 3)
            format = GL_RGB;
        else if (nrComponents == 4)
            format = GL_RGBA;

        glBindTexture(GL_TEXTURE_2D, this->id);
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, image);
        glGenerateMipmap(GL_TEXTURE_2D);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    }
    else
    {
        std::cout << "TEXTURE_LOADING_FAILED:" << file_path << std::endl;
    }

    glActiveTexture(0);
    glBindTexture(GL_TEXTURE_2D, 0);
    SOIL_free_image_data(image);
}

void Texture::bind() {
    glBindTexture(GL_TEXTURE_2D, this->id);
}

void Texture::unbind() {
    glActiveTexture(0);
    glBindTexture(GL_TEXTURE_2D, 0);
}
