#pragma once

#include "shape.h"
#include "geometry.h"
#include <glm/glm.hpp>
#include <iterator>
#include <algorithm>

class Cube : public Shape
{
public:
    Cube(unsigned int id, glm::vec4 position);
    ~Cube();

    virtual std::vector<Geometry::Intersection<Shape>> intersectRay(Ray &ray) override;
    virtual glm::vec4 normalAt(glm::vec4 point) override;
    virtual std::string type() override;

private:
};
