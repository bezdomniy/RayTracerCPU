#pragma once

#include "shape.h"
#include "geometry.h"
#include <glm/glm.hpp>

class Triangle : public Shape
{
public:
    Triangle(glm::vec3 p1, glm::vec3 p2, glm::vec3 p3);
    ~Triangle();

    virtual std::vector<Geometry::Intersection<Shape>> intersectRay(Ray &ray) override;
    virtual glm::vec4 normalAt(glm::vec4 point) override;
    virtual std::string type() override;

private:
    glm::vec3 p1;
    glm::vec3 p2;
    glm::vec3 p3;
    glm::vec3 e1;
    glm::vec3 e2;
    glm::vec4 normal;
};
