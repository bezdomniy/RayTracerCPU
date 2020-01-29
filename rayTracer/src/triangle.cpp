#include "triangle.h"

Triangle::Triangle(glm::vec3 p1, glm::vec3 p2, glm::vec3 p3)
    : Shape()
{
    this->p1 = p1;
    this->p2 = p2;
    this->p3 = p3;
    this->e1 = p2 - p1;
    this->e2 = p3 - p1;
    this->normal = glm::vec4(glm::normalize(glm::cross(e2, e1)), 0.f);
}

Triangle::~Triangle() {}

void Triangle::intersectRay(Ray& ray, std::vector<Geometry::Intersection<Shape>>& intersections) {
    Ray transformedRay = transformRay(ray);

    glm::vec3 dirCrossE2 = glm::cross(glm::vec3(transformedRay.direction), this->e2);
    float det = glm::dot(this->e1, dirCrossE2);
    if (std::abs(det) < Geometry::EPSILON)
        return;

    float f = 1.f / det;
    glm::vec3 p1ToOrigin = glm::vec3(transformedRay.origin) - this->p1;
    float u = f * glm::dot(p1ToOrigin, dirCrossE2);
    if (u < 0 || u > 1)
        return;

    glm::vec3 originCrossE1 = glm::cross(p1ToOrigin, this->e1);
    float v = f * glm::dot(glm::vec3(transformedRay.direction), originCrossE1);
    if (v < 0 || (u + v) > 1)
        return;

    float t = f * glm::dot(this->e2, originCrossE1);
    intersections.push_back(Geometry::Intersection<Shape>{t, this});
}

glm::vec4 Triangle::normalAt(glm::vec4 point)
{
    glm::vec4 worldNormal = glm::transpose(this->inverseTransform) * this->normal;
    worldNormal.w = 0.f;

    return glm::normalize(worldNormal);
}

std::string Triangle::type()
{
    return "triangle";
}