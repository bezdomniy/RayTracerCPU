#include "sphere.h"

Sphere::Sphere(unsigned int id, glm::vec4 position, float radius)
{
    this->id = id;
    this->position = position;
    this->radius = radius;
    this->transform = glm::mat4(1.f);
}

Sphere::~Sphere()
{
}

