#include "plane.h"

Plane::Plane()
    : Shape()
{
}

Plane::~Plane() {}

void Plane::intersectRay(Ray &ray, std::vector<Geometry::Intersection<Shape>> &intersections)
{
    Ray transformedRay = transformRay(ray);

    if (std::abs(transformedRay.direction.y) < Geometry::EPSILON)
        return;

    Float t = -transformedRay.origin.y / transformedRay.direction.y;

    intersections.push_back(Geometry::Intersection<Shape>{t, this});
}

Vec4 Plane::normalAt(const Vec4 &point)
{
    // Vec4 objectPoint = worldToObject(point);
    Vec4 objectNormal(0.0, 1.0, 0.0, 0.0);

    return normalToWorld(objectNormal);
}

Vec4 Plane::normalAt(const Vec4 &point, const Vec2 &uv)
{
    return normalAt(point);
}

std::pair<Vec4, Vec4> Plane::bounds()
{
    return std::pair<Vec4, Vec4>(Vec4(-std::numeric_limits<Float>::infinity(), 0., -std::numeric_limits<Float>::infinity(), 1.), Vec4(std::numeric_limits<Float>::infinity(), 0., std::numeric_limits<Float>::infinity(), 1.));
}

std::string Plane::type()
{
    return "Plane";
}
