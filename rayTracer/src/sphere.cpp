#include "sphere.h"

Sphere::Sphere(unsigned int id, glm::vec3 position, float radius)
{
    this->id = id;
    this->position = position;
    this->radius = radius;
}

Sphere::~Sphere()
{
}

