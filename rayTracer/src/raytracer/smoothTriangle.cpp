#include "smoothTriangle.h"

SmoothTriangle::SmoothTriangle(glm::dvec3 p1, glm::dvec3 p2, glm::dvec3 p3, glm::dvec3 n1, glm::dvec3 n2, glm::dvec3 n3) : Triangle(p1, p2, p3)
{
    this->n1 = n1;
    this->n2 = n2;
    this->n3 = n3;
}

SmoothTriangle::~SmoothTriangle()
{
}

// void SmoothTriangle::intersectRay(Ray &ray, std::vector<Geometry::Intersection<Shape>> &intersections)
// {
//     Ray transformedRay = transformRay(ray);

//     glm::dvec3 dirCrossE2 = glm::cross(glm::dvec3(transformedRay.direction), this->e2);
//     double det = glm::dot(this->e1, dirCrossE2);
//     if (std::abs(det) < Geometry::EPSILON)
//         return;

//     double f = 1.0 / det;
//     glm::dvec3 p1ToOrigin = glm::dvec3(transformedRay.origin) - this->p1;
//     double u = f * glm::dot(p1ToOrigin, dirCrossE2);
//     if (u < 0 || u > 1)
//         return;

//     glm::dvec3 originCrossE1 = glm::cross(p1ToOrigin, this->e1);
//     double v = f * glm::dot(glm::dvec3(transformedRay.direction), originCrossE1);
//     if (v < 0 || (u + v) > 1)
//         return;

//     double t = f * glm::dot(this->e2, originCrossE1);
//     intersections.push_back(Geometry::Intersection<Shape>{t, this, glm::dvec2(u, v)});
// }

// glm::dvec4 SmoothTriangle::normalAt(glm::dvec4 point)
// {
//     return normalToWorld(this->normal);
// }

glm::dvec4 SmoothTriangle::normalAt(const glm::dvec4 &point, const glm::dvec2 &uv)
{
    return normalToWorld(glm::dvec4(this->n2 * uv.x + this->n3 * uv.y + this->n1 * (1. - uv.x - uv.y), 0.));
}