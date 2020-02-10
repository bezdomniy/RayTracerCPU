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

    double t = -transformedRay.origin.y / transformedRay.direction.y;

    intersections.push_back(Geometry::Intersection<Shape>{t, this});

}

glm::dvec4 Plane::normalAt(glm::dvec4 point)
{
    // glm::dmat4 transformInverse(glm::affineInverse(this->transform));
    // glm::dvec4 objectPoint = transformInverse * point;
    glm::dvec4 objectNormal(0.0, 1.0, 0.0, 0.0);
    glm::dvec4 worldNormal = glm::transpose(this->inverseTransform) * objectNormal;
    worldNormal.w = 0.0;

    return glm::normalize(worldNormal);
}

std::string Plane::type()
{
    return "plane";
}