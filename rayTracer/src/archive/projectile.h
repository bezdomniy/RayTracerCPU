#pragma once

#include <glm/glm.hpp>
#include <memory>
#include "environment.h"

class Projectile
{
private:

public:
    Projectile(glm::vec4 initialPosition, glm::vec4 initialVelocity);
    Projectile(glm::vec4 initialPosition, glm::vec4 initialDirection, float initialMagnitude);
    ~Projectile();

    void setEnvironment(std::unique_ptr<Environment> &e);

    glm::vec4 position;
    glm::vec4 velocity;

    std::unique_ptr<Environment> environment;

    void tick();
};