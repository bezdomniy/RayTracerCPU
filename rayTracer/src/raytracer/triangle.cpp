#include "triangle.h"

Triangle::Triangle(glm::dvec3 p1, glm::dvec3 p2, glm::dvec3 p3)
    : Shape()
{
    this->p1 = p1;
    this->p2 = p2;
    this->p3 = p3;
    this->e1 = p2 - p1;
    this->e2 = p3 - p1;
    this->normal = glm::dvec4(glm::normalize(glm::cross(e2, e1)), 0.0);

    glm::dvec4 min(std::min({p1.x, p2.x, p3.x}), std::min({p1.y, p2.y, p3.y}), std::min({p1.z, p2.z, p3.z}), 1.);
    glm::dvec4 max(std::max({p1.x, p2.x, p3.x}), std::max({p1.y, p2.y, p3.y}), std::max({p1.z, p2.z, p3.z}), 1.);
    this->boundsv = std::pair<glm::dvec4, glm::dvec4>(min, max);
}

Triangle::~Triangle() {}

void Triangle::intersectRay(Ray &ray, std::vector<Geometry::Intersection<Shape>> &intersections)
{
    Ray transformedRay = transformRay(ray);

    glm::dvec3 dirCrossE2 = glm::cross(glm::dvec3(transformedRay.direction), this->e2);
    double det = glm::dot(this->e1, dirCrossE2);
    if (std::abs(det) < Geometry::EPSILON)
        return;

    double f = 1.0 / det;
    glm::dvec3 p1ToOrigin = glm::dvec3(transformedRay.origin) - this->p1;
    double u = f * glm::dot(p1ToOrigin, dirCrossE2);
    if (u < 0 || u > 1)
        return;

    glm::dvec3 originCrossE1 = glm::cross(p1ToOrigin, this->e1);
    double v = f * glm::dot(glm::dvec3(transformedRay.direction), originCrossE1);
    if (v < 0 || (u + v) > 1)
        return;

    double t = f * glm::dot(this->e2, originCrossE1);
    intersections.push_back(Geometry::Intersection<Shape>{t, this, glm::dvec2(u, v)});
}

glm::dvec4 Triangle::normalAt(const glm::dvec4 &point)
{
    return normalToWorld(this->normal);
}

glm::dvec4 Triangle::normalAt(const glm::dvec4 &point, const glm::dvec2 &uv)
{
    return normalAt(point);
}

inline std::pair<glm::dvec4, glm::dvec4> Triangle::bounds() const
{
    //    glm::dvec4 min(std::min({p1.x, p2.x, p3.x}), std::min({p1.y, p2.y, p3.y}), std::min({p1.z, p2.z, p3.z}), 1.);
    //    glm::dvec4 max(std::max({p1.x, p2.x, p3.x}), std::max({p1.y, p2.y, p3.y}), std::max({p1.z, p2.z, p3.z}), 1.);
    //    return std::pair<glm::dvec4, glm::dvec4>(min, max);

    return this->boundsv;
}

std::string Triangle::type()
{
    return "Triangle";
}
