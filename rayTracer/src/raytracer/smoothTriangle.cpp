#include "smoothTriangle.h"

SmoothTriangle::SmoothTriangle(glm::dvec3 p1, glm::dvec3 p2, glm::dvec3 p3, glm::dvec3 n1, glm::dvec3 n2, glm::dvec3 n3) : Triangle(p1, p2, p3)
{
    this->inverseTransform[3] = glm::dvec4(n1, 0.0);
    glm::column(this->inverseTransform, 3) = glm::dvec4(n2, 0.0);

    // this->n2 = n2;
    this->n3 = n3;
}

SmoothTriangle::~SmoothTriangle()
{
}

glm::dvec4 SmoothTriangle::normalAt(const glm::dvec4 &point, const glm::dvec2 &uv)
{
    return normalToWorld(glm::dvec4(glm::dvec3(glm::column(this->inverseTransform, 3)) * uv.x + this->n3 * uv.y + glm::dvec3(this->inverseTransform[3]) * (1. - uv.x - uv.y), 0.));
}