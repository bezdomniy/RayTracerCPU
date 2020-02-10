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

    virtual void intersectRay(Ray& ray, std::vector<Geometry::Intersection<Shape>>& intersections) override;
    virtual glm::dvec4 normalAt(glm::dvec4 point) override;
    virtual std::string type() override;

private:
};
