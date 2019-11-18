#pragma once

#include <glm/glm.hpp>
#include <memory>
#include "shape.h"

struct IntersectionParameters
{
    glm::vec4 point;
    glm::vec4 normalv;
    glm::vec4 eyev;
    glm::vec4 overPoint;
    bool inside;
};

struct Intersection
{
    float t;
    std::shared_ptr<Shape> shapePtr;
    std::unique_ptr<IntersectionParameters> comps;
};

struct Intersection
{
};