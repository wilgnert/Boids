#include "../headers/Camera.hpp"

Camera::Camera(glm::vec3 pos, glm::vec3 dir, glm::vec3 up)
:   pos(glm::normalize(pos)), front(glm::normalize(dir)), world_up(glm::normalize(up)), up(glm::normalize(up))
{
    this->view_matrix = glm::mat4(1.f);

    this->movement_speed = 3.f;
    this->sensitivity = 5.f;

    this->right = glm::normalize(glm::cross(this->up, this->front));

    this->pitch = 0.f;
    this->yaw = -90.f;
    this->roll = 0.f;
    
    this->updateCameraVectors();
}

void Camera::updateCameraVectors() {
    this->front = glm::normalize(glm::vec3(
        cos(glm::radians(this->yaw)) * cos(glm::radians(this->pitch)),
        sin(glm::radians(this->pitch)),
        sin(glm::radians(this->yaw)) * cos(glm::radians(this->pitch))
    ));

    this->right = glm::normalize(glm::cross(this->front, this->world_up));

    this->up = glm::normalize(glm::cross(this->right, this->front));
}

const glm::mat4 Camera::getViewMatrix() {
    this->view_matrix = glm::lookAt(this->pos, this->pos + this->front, this->up);

    this->updateCameraVectors();

    return this->view_matrix;
}

const glm::vec3 Camera::getPosition() const {
    return this->pos;
}

void Camera::updateInput(const float& dt, const glm::dvec2 offset) {
    this->updateMouseInput(dt, offset);
}

void Camera::updateMouseInput(const float& dt, const glm::dvec2 offset) {
    // update pitch, yaw & roll
    this->pitch += static_cast<float>(offset.y) * dt * this->sensitivity;
    this->yaw += static_cast<float>(offset.x) * dt * this->sensitivity;
    
    if (this->pitch > 80.f) this->pitch = 80.f;
    else if (this->pitch < -80.f) this->pitch = -80.f;

    if (yaw > 360.f || yaw < -360.f) yaw = 0.f;
}
void Camera::move(const float &dt, const int direction) {
    // update position vector
    switch (direction)
    {
    case Direction::FOWARD:
        this->pos += this->front * this->movement_speed * dt;
        break;
    case Direction::BACKWARD:
        this->pos -= this->front * this->movement_speed * dt;
        break;
    case Direction::LEFT:
        this->pos -= this->right * this->movement_speed * dt;
        break;
    case Direction::RIGHT:
        this->pos += this->right * this->movement_speed * dt;
        break;
    case Direction::UPWARD:
        this->pos += this->up * this->movement_speed * dt;
        break;  
    case Direction::DOWNWARD:
        this->pos -= this->up * this->movement_speed * dt;
        break; 
    default:
        break;
    }
}