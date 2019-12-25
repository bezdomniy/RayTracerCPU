#pragma once

#include "shape.h"
#include "geometry.h"
#include <glm/glm.hpp>
// #include <vector>

class Plane : public Shape
{
public:
    Plane();
    ~Plane();

    virtual std::vector<Geometry::Intersection<Shape>> intersectRay(Ray &ray) override;
    virtual glm::vec4 normalAt(glm::vec4 point) override;
    virtual std::string type() override;

private:
};
