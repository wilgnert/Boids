#include "../headers/Game.hpp"

// Private functions
void Game::initGLFW() {
    /* Initialize the GLFW library */
    if (glfwInit() == GLFW_FALSE) {
        std::cout << "Failed to initialize GLFW" << std::endl;
        exit(-1);
    }
}
void Game::initWindow(bool resizable) 
{
    /* Create a windowed mode window and its OpenGL context */
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, this->GL_VERSION_MAJOR);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, this->GL_VERSION_MINOR);
	glfwWindowHint(GLFW_RESIZABLE, resizable);

    this->window = glfwCreateWindow(this->WINDOW_WIDTH, this->WINDOW_HEIGHT, this->TITLE, NULL, NULL);
    
    if (!this->window)
    {
        std::cout << "Failed to initialize GLFW window" << std::endl;
        glfwTerminate();
        exit(-1);
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(this->window);
}
void Game::initGLAD() {
    /* Initialize GLAD */
    if (!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress)) {
        std::cout << "Failed to initialize OpenGL context" << std::endl;
        glfwTerminate();
        exit(-1);
    }
}
void Game::initOpenGLOptions() {
    glEnable(GL_DEPTH_TEST);

    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    glFrontFace(GL_CCW);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glPolygonMode(GL_FRONT, GL_FILL);
    glPolygonMode(GL_BACK, GL_LINE);

    // glfwSetInputMode(this->window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
}
void Game::initMatrices() {
    this->view_matrix = this->camera.getViewMatrix();

    this->fov = 90.f;
    this->near_plane = 0.1f;
    this->far_plane = 1000.f;
    this->projection_matrix = glm::mat4(1.f);
    this->projection_matrix = glm::perspective(
        glm::radians(this->fov),
        16.f / 9.f,
        this->near_plane,
        this->far_plane
    );
}
void Game::initRuntimeVariables() {
    glfwSetFramebufferSizeCallback(this->window, Game::framebufferResizeCallback);
    glfwGetFramebufferSize(this->window, &this->fbwidth, &this->fbheight);
    if (fbwidth == 0) { fbwidth = 1; }
    if (fbheight == 0) { fbheight = 1; }
    glViewport(0, 0, this->fbwidth, this->fbheight);
    glfwGetWindowSize(this->window, &fbwidth, &fbheight);
    glfwSetWindowAspectRatio(this->window, fbwidth, fbheight);
}
void Game::initShaders() {
    this->shaders.push_back(new Shader(this->GL_VERSION_MAJOR, this->GL_VERSION_MINOR, "C:\\Users\\absit\\Documents\\CG\\Boids\\src\\shaders\\vertex\\core.glsl", "C:\\Users\\absit\\Documents\\CG\\Boids\\src\\shaders\\fragment\\core.glsl"));
}
void Game::initTextures() {    
    this->textures.push_back(new Texture("C:\\Users\\absit\\Documents\\CG\\Boids\\images\\amongus.png", "texture_diffuse"));
    this->textures.push_back(new Texture("C:\\Users\\absit\\Documents\\CG\\Boids\\images\\amongus.png", "texture_specular"));
    this->textures.push_back(new Texture("C:\\Users\\absit\\Documents\\CG\\Boids\\images\\spdrmn.png", "texture_diffuse"));
    this->textures.push_back(new Texture("C:\\Users\\absit\\Documents\\CG\\Boids\\images\\spdrmn.png", "texture_specular"));
    this->textures.push_back(new Texture("C:\\Users\\absit\\Documents\\CG\\Boids\\fish\\fish_texture.png", "texture_diffuse"));
    this->textures.push_back(new Texture("C:\\Users\\absit\\Documents\\CG\\Boids\\fish\\fish_texture.png", "texture_specular"));
}
void Game::initMaterials() {
    this->materials.push_back(new Material(
        glm::vec3(0.3f),
        glm::vec3(1.f),
        glm::vec3(2.f),
        0,
        0));
}
void Game::initLights() {
    this->lights.push_back(new glm::vec3(1.f));
}
void Game::initModels() {
    Cube cube;
    std::vector<Texture> texts;
    texts.push_back(*(this->textures[Textures::AMONGUS_DIFFUSE]));
    texts.push_back(*(this->textures[Textures::AMONGUS_SPECULAR]));
    this->meshes.push_back(
        new Mesh(
            cube.getVertices(),
            cube.getIndices(),
            std::vector<Texture>()
        )
    );
    texts.clear();
    this->models.push_back(new Model(
        glm::vec3(0.f),
        this->materials[Materials::MAT_1],
        this->meshes,
        this->textures[Textures::AMONGUS_DIFFUSE],
        this->textures[Textures::AMONGUS_SPECULAR]
        )
    );

    this->meshes.clear();
    
    
    this->models.push_back(new Model(
        glm::fvec3(0.f, 0.f, 0.f),
        this->materials[Materials::MAT_1],
        "fish/fish.obj",
        this->textures[Textures::FISH_DIFFUSE],
        this->textures[Textures::FISH_SPECULAR])
    );
}
void Game::initUniforms() {
    this->shaders[ShaderPrograms::CORE]->setMat4("view_matrix", this->view_matrix, GL_FALSE);
    this->shaders[ShaderPrograms::CORE]->setMat4("projection_matrix", this->projection_matrix, GL_FALSE);
    
    this->shaders[ShaderPrograms::CORE]->setVec3("light_pos_0", *(this->lights[0]));
    this->shaders[ShaderPrograms::CORE]->setVec3("camera_pos",this->camera.getPosition());
}

void Game::updateInput() {
    glfwPollEvents();
    updateKeyboardInput();
    updateMouseInput();
    this->camera.updateInput(this->delta.dt, this->mouse.offset);
}
void Game::updateMouseInput() {
    glfwGetCursorPos(this->window, &this->mouse.pos.x, &this->mouse.pos.y);
    
    if (this->mouse.first)
    {
        this->mouse.last_pos = this->mouse.pos;
        this->mouse.first = false;
    }
    
    this->mouse.offset.x = this->mouse.pos.x - this->mouse.last_pos.x;
    this->mouse.offset.y = this->mouse.last_pos.y - this->mouse.pos.y;

    this->mouse.last_pos = this->mouse.pos;
}
void Game::updateKeyboardInput() {
    
    if (glfwGetKey(this->window, GLFW_KEY_LEFT_ALT) == GLFW_PRESS) {
        glfwSetInputMode(this->window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
        this->mouse.pos = glm::dvec2(0.0);
        this->mouse.last_pos = glm::dvec2(0.0);
        this->mouse.first = true;
    } 
    else {
        glfwSetInputMode(this->window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    }
    
    if (glfwGetKey(this->window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        this->setWindowShouldClose();
    }
    
    // TRANSLATION  
    // Z Axis translation  
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
        this->camera.move(this->delta.dt, Direction::BACKWARD);
    }
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
        this->camera.move(this->delta.dt, Direction::FOWARD);
    }
    // Y Axis translation
    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) {
        this->camera.move(this->delta.dt, Direction::UPWARD);
    } if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) {
        this->camera.move(this->delta.dt, Direction::DOWNWARD);
    }
    // X Axis translation
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
        this->camera.move(this->delta.dt, Direction::RIGHT);
    }
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
        this->camera.move(this->delta.dt, Direction::LEFT);
    }
}
void Game::updateMatrices() {
    // Update view matrix
    this->view_matrix = this->camera.getViewMatrix();

    // update projection matrix
    this->projection_matrix = glm::perspective(
        glm::radians(this->fov), 
        static_cast<float>(this->fbwidth)/this->fbheight, 
        this->near_plane, 
        this->far_plane);
}
void Game::updateUniforms() {
    this->shaders[ShaderPrograms::CORE]->setMat4("view_matrix", this->view_matrix, GL_FALSE);
    this->shaders[ShaderPrograms::CORE]->setVec3("camera_pos",this->camera.getPosition());
    this->shaders[ShaderPrograms::CORE]->setMat4("projection_matrix", this->projection_matrix, GL_FALSE);

}
void Game::updateModels() {
    for (auto& m : models)
        m->update();
}
void Game::updateDt() {
    this->delta.update();
}


// Constructor
Game::Game(
    const char *TITLE,
    const int W, const int H,
    const int GL_MAJOR, const int GL_MINOR,
    bool resizable)
:   TITLE(TITLE),
    WINDOW_WIDTH(W),
    WINDOW_HEIGHT(H),
    GL_VERSION_MAJOR(GL_MAJOR),
    GL_VERSION_MINOR(GL_MINOR),
    camera(glm::vec3(0.f, 0.f, 2.f), glm::vec3(0.f, 0.f, -1.f), glm::vec3(0.f, 1.f, 0.f))
{
    this->initGLFW();
    this->initWindow(resizable);
    this->initGLAD();
    this->initOpenGLOptions();
    this->initMatrices();
    this->initShaders();
    this->initTextures();
    this->initMaterials();
    this->initModels();
    this->initLights();
    this->initUniforms();
}

Game::~Game() {
    if (this->window != nullptr) glfwDestroyWindow(this->window);
    for (auto& s : this->shaders)
        delete s;
    for (auto& t : this->textures)
        delete t;
    for (auto& m : this->materials)
        delete m;
    for (auto& m : this->meshes)
        delete m;
    for (auto& l : this->lights)
        delete l;
    glfwTerminate();
}

// Getters
int Game::getWindowShouldClose() {
    return glfwWindowShouldClose(this->window);
}
GLFWwindow *Game::getWindow() {
    return this->window;
}

// Setters
void Game::setWindowShouldClose(bool value) {
    glfwSetWindowShouldClose(this->window, value);
}

// Fuctions
void Game::update() {
    this->updateDt();
    updateInput();
    
    glfwGetFramebufferSize(this->window, &fbwidth, &fbheight);
    if (fbwidth == 0) { fbwidth = 1; }
    if (fbheight == 0) { fbheight = 1; }
    glViewport(0, 0, fbwidth, fbheight);
    
    updateMatrices();
    updateUniforms();
    updateModels();
    this->models[0]->circularMotion(1.39);
}
void Game::render() {
    // START DRAW
    glClearColor(0.f, 0.4f, 0.49f, 1.f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
    
    // DRAW

    for (auto& m : models)
        m->render(this->shaders[ShaderPrograms::CORE]);

    // END DRAW
    glfwSwapBuffers(this->window);
    glFlush();

    glBindVertexArray(0);
    glUseProgram(0);
    glActiveTexture(0);
    glBindTexture(GL_TEXTURE_2D, 0);
}

// Static functions
void Game::framebufferResizeCallback(GLFWwindow * win, GLint fb_w, GLint fb_h) {
    if (fb_w == 0) { fb_w = 1; }
    if (fb_h == 0) { fb_h = 1; }
    
    glViewport(0, 0, fb_w, fb_h);
}