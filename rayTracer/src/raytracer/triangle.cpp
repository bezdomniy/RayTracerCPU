#include "triangle.h"

Triangle::Triangle(Vec3 p1, Vec3 p2, Vec3 p3)
    : Shape()
{
    this->inverseTransform[0] = Vec4(p1, 1.0);
    this->inverseTransform[1] = Vec4(p2, 1.0);
    this->inverseTransform[2] = Vec4(p3, 1.0);
    this->inverseTransform[3] = Vec4(glm::normalize(glm::cross(p3 - p1, p2 - p1)), 0.0);

    Vec4 min(std::min({p1.x, p2.x, p3.x}), std::min({p1.y, p2.y, p3.y}), std::min({p1.z, p2.z, p3.z}), 1.);
    Vec4 max(std::max({p1.x, p2.x, p3.x}), std::max({p1.y, p2.y, p3.y}), std::max({p1.z, p2.z, p3.z}), 1.);
    this->boundsv = std::pair<Vec4, Vec4>(min, max);
}

Triangle::~Triangle() {}

void Triangle::intersectRay(Ray &ray, std::vector<Geometry::Intersection<Shape>> &intersections)
{
    // Ray transformedRay = transformRay(ray);

    Vec3 e1 = this->inverseTransform[1] - this->inverseTransform[0];
    Vec3 e2 = this->inverseTransform[2] - this->inverseTransform[0];

    Vec3 dirCrossE2 = glm::cross(Vec3(ray.direction), e2);
    Float det = glm::dot(e1, dirCrossE2);
    if (std::abs(det) < Geometry::EPSILON)
        return;

    Float f = 1.0 / det;
    Vec3 p1ToOrigin = Vec3(ray.origin) - Vec3(this->inverseTransform[0]);
    Float u = f * glm::dot(p1ToOrigin, dirCrossE2);
    if (u < 0 || u > 1)
        return;

    Vec3 originCrossE1 = glm::cross(p1ToOrigin, e1);
    Float v = f * glm::dot(Vec3(ray.direction), originCrossE1);
    if (v < 0 || (u + v) > 1)
        return;

    Float t = f * glm::dot(e2, originCrossE1);
    intersections.push_back(Geometry::Intersection<Shape>{t, this, Vec2(u, v)});
}

Vec4 Triangle::normalAt(const Vec4 &point)
{
    return normalToWorld(this->inverseTransform[3]);
}

Vec4 Triangle::normalAt(const Vec4 &point, const Vec2 &uv)
{
    return normalAt(point);
}

inline std::pair<Vec4, Vec4> Triangle::bounds() const
{
    //    Vec4 min(std::min({p1.x, p2.x, p3.x}), std::min({p1.y, p2.y, p3.y}), std::min({p1.z, p2.z, p3.z}), 1.);
    //    Vec4 max(std::max({p1.x, p2.x, p3.x}), std::max({p1.y, p2.y, p3.y}), std::max({p1.z, p2.z, p3.z}), 1.);
    //    return std::pair<Vec4, Vec4>(min, max);

    return this->boundsv;
}

std::string Triangle::type()
{
    return "Triangle";
}
