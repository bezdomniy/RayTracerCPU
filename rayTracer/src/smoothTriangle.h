#pragma once

#include "triangle.h"

class SmoothTriangle : public Triangle
{
private:
    glm::dvec3 n1;
    glm::dvec3 n2;
    glm::dvec3 n3;

public:
    SmoothTriangle(glm::dvec3 p1, glm::dvec3 p2, glm::dvec3 p3, glm::dvec3 n1, glm::dvec3 n2, glm::dvec3 n3);
    ~SmoothTriangle();

    virtual void intersectRay(Ray &ray, std::vector<Geometry::Intersection<Shape>> &intersections) override;

    // TODO make this match with other normalAts..
    virtual glm::dvec4 normalAt(glm::dvec4 point, glm::dvec2 uv) override;
};
