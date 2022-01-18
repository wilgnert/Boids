#ifndef MODEL_H
#define MODEL_H

#include <string>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "Mesh.hpp"
#include "OBJLoader.hpp"

class Model
{
private:
    Material *material;
    Texture *overrideTextureDiffuse;
    Texture *overrideTextureSpecular;
    std::vector<Mesh> meshes;
    std::vector<Texture> textures_loaded;
    glm::vec3 position;
    std::string directory;

    void updateUniforms();
    void loadModel(std::string path);
    void processNode(aiNode *node, const aiScene *scene);
    Mesh processMesh(aiMesh *mesh, const aiScene *scene);
    std::vector<Texture> loadMaterialTextures(aiMaterial *mat, aiTextureType type, std::string typeName);

public:
    Model(
        glm::vec3 position,
        Material *material,
        std::vector<Mesh *> meshes,
        Texture *overrideTextureDiffuse = nullptr,
        Texture *overrideTextureSpecular = nullptr
    );
    // OBJ File Loaded Model
    Model(
        glm::vec3 position,
        Material *material,
        const char* obj_file_path,
        Texture *overrideTextureDiffuse = nullptr,
        Texture *overrideTextureSpecular = nullptr

    );
    ~Model();

    void update();
    void render(Shader *shader);

    void move(glm::vec3 tsl);
    void rotate(glm::vec3 rtt);
    void scale(glm::vec3 scl);

    void circularMotion(double radius);
};

#endif // MODEL_H