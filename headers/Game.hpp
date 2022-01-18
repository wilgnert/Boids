#ifndef GAME_H
#define GAME_H

#include "libs.hpp"

// ENUMERATIONS
enum ShaderPrograms
{
    CORE = 0
};
enum Textures
{
    AMONGUS_DIFFUSE = 0,
    AMONGUS_SPECULAR,
    MIRANHA_DIFFUSE, 
    MIRANHA_SPECULAR, 
    FISH_DIFFUSE,
    FISH_SPECULAR
};
enum Materials
{
    MAT_1 = 0
};
enum Meshes
{
    CUBE = 0
};

class Game
{
private:
    /* data */
// Variables
    struct DeltaTime {
        double FPS;
        float dt;
        float cur_time;
        float last_time;

        DeltaTime() : dt(0.f), cur_time(0.f), last_time(0.f), FPS(60) {}
        void update() {
            this->cur_time = static_cast<float>(glfwGetTime());
            this->dt = this->cur_time - this->last_time;
            while (dt < 1.0 / FPS)
            {
                this->cur_time = static_cast<float>(glfwGetTime());
                this->dt = this->cur_time - this->last_time;
            }
            this->last_time = this->cur_time;
        }
    } delta;

    struct MouseData {
        glm::dvec2 last_pos;
        glm::dvec2 pos;
        glm::dvec2 offset;
        bool first;

        MouseData() : last_pos(0.f), pos(0.f), offset(0.f), first(true) { }
    } mouse;

    Camera camera;

    // Window
    GLFWwindow *window;
    const char *TITLE;
    const int WINDOW_WIDTH;
    const int WINDOW_HEIGHT;
    int fbwidth;
    int fbheight;
    // OpenGL context
    const int GL_VERSION_MAJOR;
    const int GL_VERSION_MINOR;
    //Matrices
    glm::vec3 world_up;
    glm::vec3 cam_front;
    glm::vec3 cam_pos;
    glm::mat4 view_matrix;

    float fov;
    float near_plane;
    float far_plane;
    glm::mat4 projection_matrix;

    // Shaders
    std::vector<Shader*> shaders;


    // Materials
    std::vector<Material*> materials;

    // Meshes
    std::vector<Mesh*> meshes;

    // Lights
    std::vector<glm::vec3*> lights;

    // Models
    std::vector<Model *> models;

    // Private functions
    // Init functions
    void initGLFW();
    void initWindow(bool resizable);
    void initGLAD();
    void initOpenGLOptions();
    void initMatrices();
    void initRuntimeVariables();
    void initShaders();
    void initTextures();
    void initMaterials();
    void initLights();
    void initModels();
    void initUniforms();

    // Update functions
    void updateInput();
    void updateKeyboardInput();
    void updateMouseInput();
    void updateMatrices();
    void updateUniforms();
    void updateModels();
    void updateDt();
    // Static variables

public:
    // Textures
    std::vector<Texture*> textures;
    Game(const char* TITLE,
        const int W, const int H,
        const int GL_MAJOR, const int GL_MINOR,
        bool resizable
    );
    virtual ~Game();

// Getters
    int getWindowShouldClose();
    GLFWwindow *getWindow();

// Setters
    void setWindowShouldClose(bool value = GLFW_TRUE);

// Fuctions
    void update();
    void render();

// Static functions
    static void framebufferResizeCallback(GLFWwindow * win, GLint fb_w, GLint fb_h);
};

#endif // GAME_H