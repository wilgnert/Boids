#ifndef BOID_H
#define BOID_H

#include <glm/vec3.hpp>
#include <../headers/Model.hpp>

class Boid
{
private:
    double calculateDist(Boid &that);
    glm::vec3 calculateAttraction(Boid &that);
    glm::vec3 calculateRepulsion(Boid &that);

    glm::vec3 pos;
    glm::vec3 front;
    glm::vec3 up;
    glm::vec3 right;

    double vel;

    static const double MIN_DIST = 0.1;

public:
    Boid(/* args */);
    ~Boid();

    void update();
    void render();
};

Boid::Boid(/* args */)
{
}

Boid::~Boid()
{
}




#endif // BOID_H