#include "smoothTriangle.h"

SmoothTriangle::SmoothTriangle(Vec3 p1, Vec3 p2, Vec3 p3, Vec3 n1, Vec3 n2, Vec3 n3) : Triangle(p1, p2, p3)
{
    this->n1 = n1;
    this->n2 = n2;
    this->n3 = n3;
}

SmoothTriangle::~SmoothTriangle()
{
}

void SmoothTriangle::intersectRay(Ray &ray, std::vector<Geometry::Intersection<Shape>> &intersections)
{
    Ray transformedRay = transformRay(ray);

    Vec3 dirCrossE2 = glm::cross(Vec3(transformedRay.direction), this->e2);
    Float det = glm::dot(this->e1, dirCrossE2);
    if (std::abs(det) < Geometry::EPSILON)
        return;

    Float f = 1.0 / det;
    Vec3 p1ToOrigin = Vec3(transformedRay.origin) - this->p1;
    Float u = f * glm::dot(p1ToOrigin, dirCrossE2);
    if (u < 0 || u > 1)
        return;

    Vec3 originCrossE1 = glm::cross(p1ToOrigin, this->e1);
    Float v = f * glm::dot(Vec3(transformedRay.direction), originCrossE1);
    if (v < 0 || (u + v) > 1)
        return;

    Float t = f * glm::dot(this->e2, originCrossE1);
    intersections.push_back(Geometry::Intersection<Shape>{t, this, Vec2(u, v)});
}

// Vec4 SmoothTriangle::normalAt(Vec4 point)
// {
//     return normalToWorld(this->normal);
// }

Vec4 SmoothTriangle::normalAt(const Vec4 &point, const Vec2 &uv)
{
    return normalToWorld(Vec4(this->n2 * uv.x + this->n3 * uv.y + this->n1 * ((Float)1 - uv.x - uv.y), (Float)0));
}