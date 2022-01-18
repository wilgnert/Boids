#include "../headers/Model.hpp"

unsigned int TextureFromFile(const char *path, const std::string &directory)
{
    std::string filename = std::string(path);
    filename = directory + '/' + filename;

    unsigned int textureID;
    glGenTextures(1, &textureID);

    int width, height, nrComponents;
    unsigned char *data = SOIL_load_image(filename.c_str(), &width, &height, &nrComponents, 0);
    if (data)
    {
        GLenum format;
        if (nrComponents == 1)
            format = GL_RED;
        else if (nrComponents == 3)
            format = GL_RGB;
        else if (nrComponents == 4)
            format = GL_RGBA;

        glBindTexture(GL_TEXTURE_2D, textureID);
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        SOIL_free_image_data(data);
    }
    else
    {
        std::cout << "Texture failed to load at path: " << path << std::endl;
        SOIL_free_image_data(data);
    }

    return textureID;
}

void Model::loadModel(std::string path) {
    Assimp::Importer import;
    const aiScene *scene = import.ReadFile(path, aiProcess_Triangulate);	
	
    if(!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) 
    {
        std::cout << "ERROR::ASSIMP::" << import.GetErrorString() << std::endl;
        return;
    }
    directory = path.substr(0, path.find_last_of('/'));

    processNode(scene->mRootNode, scene);
}
void Model::processNode(aiNode *node, const aiScene *scene) {
    // process all the node's meshes (if any)
    for(unsigned int i = 0; i < node->mNumMeshes; i++)
    {
        aiMesh *mesh = scene->mMeshes[node->mMeshes[i]]; 
        meshes.push_back(processMesh(mesh, scene));			
    }
    // then do the same for each of its children
    for(unsigned int i = 0; i < node->mNumChildren; i++)
    {
        processNode(node->mChildren[i], scene);
    }
}
Mesh Model::processMesh(aiMesh *mesh, const aiScene *scene) {
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;
    std::vector<Texture> textures;

    for(unsigned int i = 0; i < mesh->mNumVertices; i++)
    {
        Vertex vertex;
        glm::vec3 vector; 
        // process vertex positions, normals and texture coordinates
        
        vector.x = mesh->mVertices[i].x;
        vector.y = mesh->mVertices[i].y;
        vector.z = mesh->mVertices[i].z; 
        vertex.position = vector;

        vector.x = mesh->mNormals[i].x;
        vector.y = mesh->mNormals[i].y;
        vector.z = mesh->mNormals[i].z;
        vertex.normal = vector; 

        if(mesh->mTextureCoords[0]) // does the mesh contain texture coordinates?
        {
            glm::vec2 vec;
            vec.x = mesh->mTextureCoords[0][i].x; 
            vec.y = mesh->mTextureCoords[0][i].y;
            vertex.texture_coord = vec;
        }
        else
            vertex.texture_coord = glm::vec2(0.0f, 0.0f);

        vertices.push_back(vertex);
    }
    // process indices
    for(unsigned int i = 0; i < mesh->mNumFaces; i++)
    {
        aiFace face = mesh->mFaces[i];
        for(unsigned int j = 0; j < face.mNumIndices; j++)
            indices.push_back(face.mIndices[j]);
    }
    // process material
    if(mesh->mMaterialIndex >= 0)
    {
        aiMaterial *material = scene->mMaterials[mesh->mMaterialIndex];
        std::vector<Texture> diffuseMaps = loadMaterialTextures(material, 
                                            aiTextureType_DIFFUSE, "texture_diffuse");
        textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());
        std::vector<Texture> specularMaps = loadMaterialTextures(material, 
                                            aiTextureType_SPECULAR, "texture_specular");
        textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
    }

    return Mesh(vertices, indices, textures);
}

std::vector<Texture> Model::loadMaterialTextures(aiMaterial *mat, aiTextureType type, std::string typeName)
{
    std::vector<Texture> textures;
    for(unsigned int i = 0; i < mat->GetTextureCount(type); i++)
    {
        aiString str;
        mat->GetTexture(type, i, &str);
        bool skip = false;
        for(unsigned int j = 0; j < textures_loaded.size(); j++)
        {
            if(std::strcmp(textures_loaded[j].path.data(), str.C_Str()) == 0)
            {
                textures.push_back(textures_loaded[j]);
                skip = true; 
                break;
            }
        }
        if(!skip)
        {   // if texture hasn't been loaded already, load it
            Texture texture;
            texture.id = TextureFromFile(str.C_Str(), this->directory);
            texture.type = typeName;
            texture.path = str.C_Str();
            textures.push_back(texture);
            textures_loaded.push_back(texture); // add to loaded textures
        }
    }
    return textures;
} 

Model::Model(
    glm::vec3 position,
    Material *material, 
    std::vector<Mesh *> meshes,
    Texture *overrideTextureDiffuse,
    Texture *overrideTextureSpecular
) 
:   position(position), material(material), 
    overrideTextureDiffuse(overrideTextureDiffuse),
    overrideTextureSpecular(overrideTextureSpecular)
{ 
    for(auto& i : meshes) {
        this->meshes.push_back(Mesh(*i));
    }
    for (auto& i : this->meshes) {
        i.move(this->position);
        i.setOrigin(this->position);
    }
}
// OBJ File Loaded Model
Model::Model(
    glm::vec3 position,
    Material *material,
    const char* obj_file_path,
    Texture *overrideTextureDiffuse,
    Texture *overrideTextureSpecular
) : position(position), material(material),
    overrideTextureDiffuse(overrideTextureDiffuse),
    overrideTextureSpecular(overrideTextureSpecular)
{
    loadModel(obj_file_path);
    
    for (auto& i : this->meshes) {
        i.setPosition(this->position);
        i.setOrigin(this->position);
    }
    this->update();
}

Model::~Model()
{
}

void Model::update() {
    for (auto& i : this->meshes)
        i.update();
}
void Model::render(Shader *shader) {
    this->material->sendToShader(*(shader));
    shader->use();

    for (auto& i : this->meshes) {
        if (this->overrideTextureDiffuse != nullptr) {
            this->overrideTextureDiffuse->bind();            
        }
        if (this->overrideTextureSpecular != nullptr) {
            this->overrideTextureSpecular->bind();
        }
        i.render(shader);
    }    
}

void Model::move(glm::vec3 tsl) {
    for (auto& m : meshes) m.move(tsl);
}
void Model::rotate(glm::vec3 rtt) {
    for (auto& m : meshes) m.rotate(rtt);
}
void Model::scale(glm::vec3 scl) {
    for (auto& m : meshes) m.scale(scl);
}

void Model::circularMotion(double radius) {
    this->meshes[0].setPosition(
        glm::vec3(
            radius * cos(glfwGetTime()),
            radius * (sin(glm::golden_ratio<double>() * glfwGetTime()) + cos(glm::golden_ratio<double>() * glfwGetTime())),
            radius * sin(glfwGetTime())
            ) - this->position
        );
    }