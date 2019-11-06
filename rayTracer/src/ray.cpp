#include "ray.h"

Ray::Ray(glm::vec3 origin, glm::vec3 direction)
{
    this->origin = origin;
    this->direction = direction;
}

Ray::~Ray()
{
}

glm::vec3 Ray::positionAtTime(float t)
{
    return origin + direction * t;
}