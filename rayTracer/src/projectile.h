#pragma once

#include <glm/glm.hpp>
#include <memory>
#include "environment.h"

class Projectile
{
private:

public:
    Projectile(glm::vec3 initialPosition, glm::vec3 initialVelocity);
    Projectile(glm::vec3 initialPosition, glm::vec3 initialDirection, float initialMagnitude);
    ~Projectile();

    void setEnvironment(std::unique_ptr<Environment> &e);

    glm::vec3 position;
    glm::vec3 velocity;

    std::unique_ptr<Environment> environment;

    void tick();
};