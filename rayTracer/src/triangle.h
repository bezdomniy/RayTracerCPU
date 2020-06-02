#pragma once

#include "shape.h"
#include "geometry.h"
#include <glm/glm.hpp>

class Triangle : public Shape
{
public:
    Triangle(glm::dvec3 p1, glm::dvec3 p2, glm::dvec3 p3);
    ~Triangle();

    virtual void intersectRay(Ray &ray, std::vector<Geometry::Intersection<Shape>> &intersections) override;
    virtual glm::dvec4 normalAt(glm::dvec4 point) override;
    virtual glm::dvec4 normalAt(glm::dvec4 point, glm::dvec2 uv) override;
    virtual std::pair<glm::dvec4, glm::dvec4> bounds() override;
    virtual std::string type() override;

protected:
    glm::dvec3 p1;
    glm::dvec3 p2;
    glm::dvec3 p3;
    glm::dvec3 e1;
    glm::dvec3 e2;
    glm::dvec4 normal;
};
