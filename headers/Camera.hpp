#ifndef CAMERA_H
#define CAMERA_H

#include <iostream>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>

enum Direction
{
    FOWARD = 0,
    BACKWARD,
    LEFT,
    RIGHT,
    UPWARD,
    DOWNWARD
};

class Camera 
{
private:
    glm::mat4 view_matrix;

    GLfloat movement_speed, sensitivity;

    glm::vec3 pos, front, right, up, world_up;

    GLfloat pitch, yaw, roll;

    void updateCameraVectors();

public:
    Camera(glm::vec3 pos, glm::vec3 dir, glm::vec3 world_up);

    // Getters
    const glm::mat4 getViewMatrix();
    const glm::vec3 getPosition() const;

    // Fuctions
    void updateInput(const float& dt, const glm::dvec2 offset);

    void updateMouseInput(const float& dt, const glm::dvec2 offset);

    void move(const float &dt, const int direction);
};

#endif // CAMERA_H