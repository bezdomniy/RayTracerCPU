#include "plane.h"

Plane::Plane(unsigned int id, glm::vec4 position)
    : Shape(id, position)
{
    this->transform = glm::mat4(1.f);
}

Plane::~Plane() {}

std::vector<Geometry::Intersection<Shape>> Plane::intersectRay(Ray &ray)
{
    std::vector<Geometry::Intersection<Shape>> ret;
    return ret;
}