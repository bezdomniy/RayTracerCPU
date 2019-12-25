#include "plane.h"

Plane::Plane()
    : Shape()
{
}

Plane::~Plane() {}

std::vector<Geometry::Intersection<Shape>> Plane::intersectRay(Ray &ray)
{
    Ray transformedRay = transformRay(ray);

    std::vector<Geometry::Intersection<Shape>> ret;
    if (std::abs(transformedRay.direction.y) < Geometry::EPSILON)
        return ret;

    float t = -transformedRay.origin.y / transformedRay.direction.y;

    ret.push_back(Geometry::Intersection<Shape>{t, this});

    return ret;
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