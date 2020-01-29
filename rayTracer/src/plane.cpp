#include "plane.h"

Plane::Plane()
    : Shape()
{
}

Plane::~Plane() {}

void Plane::intersectRay(Ray& ray, std::vector<Geometry::Intersection<Shape>>& intersections)
{
    Ray transformedRay = transformRay(ray);

    if (std::abs(transformedRay.direction.y) < Geometry::EPSILON)
        return;

    float t = -transformedRay.origin.y / transformedRay.direction.y;

    intersections.push_back(Geometry::Intersection<Shape>{t, this});

}

glm::vec4 Plane::normalAt(glm::vec4 point)
{
    // glm::mat4 transformInverse(glm::affineInverse(this->transform));
    // glm::vec4 objectPoint = transformInverse * point;
    glm::vec4 objectNormal(0.f, 1.f, 0.f, 0.f);
    glm::vec4 worldNormal = glm::transpose(this->inverseTransform) * objectNormal;
    worldNormal.w = 0.f;

    return glm::normalize(worldNormal);
}

std::string Plane::type()
{
    return "plane";
}