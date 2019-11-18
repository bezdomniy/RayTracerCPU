#pragma once

#include "shape.h"
#include "geometry.h"
#include <glm/glm.hpp>
// #include <vector>

class Plane : public Shape
{
public:
    Plane(unsigned int id, glm::vec4 position);
    ~Plane();

    std::vector<Geometry::Intersection<Shape>> intersectRay(Ray &ray);

private:
};
