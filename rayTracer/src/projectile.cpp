#include "projectile.h"

Projectile::Projectile(glm::vec4 initialPosition, glm::vec4 initialVelocity)
{
    position = initialPosition;
    velocity = initialVelocity;
}

Projectile::Projectile(glm::vec4 initialPosition, glm::vec4 initialDirection, float initialMagnitude) {
    position = initialPosition;
    velocity = glm::normalize(initialDirection) * initialMagnitude;
}

Projectile::~Projectile()
{
}

void Projectile::tick() {
    position = position + velocity;
    velocity = velocity + environment->gravity + environment->wind;
}

void Projectile::setEnvironment(std::unique_ptr<Environment> &e) {
    environment = std::move(e);
}