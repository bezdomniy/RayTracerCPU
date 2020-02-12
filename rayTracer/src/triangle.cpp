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
}

Triangle::~Triangle() {}

void Triangle::intersectRay(Ray& ray, std::vector<Geometry::Intersection<Shape>>& intersections) {
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
    intersections.push_back(Geometry::Intersection<Shape>{t, this});
}

glm::dvec4 Triangle::normalAt(glm::dvec4 point)
{
    // glm::dvec4 worldNormal = glm::transpose(this->inverseTransform) * this->normal;
    // worldNormal.w = 0.0;

    return normalToWorld(this->normal);
}

std::pair<glm::dvec4,glm::dvec4> Triangle::bounds() {
    glm::dvec4 min(std::min({p1.x,p2.x,p3.x}),std::min({p1.y,p2.y,p3.y}),std::min({p1.z,p2.z,p3.z}),1.);
    glm::dvec4 max(std::max({p1.x,p2.x,p3.x}),std::max({p1.y,p2.y,p3.y}),std::max({p1.z,p2.z,p3.z}),1.);
  return std::pair<glm::dvec4,glm::dvec4>(min,max);
}

std::string Triangle::type()
{
    return "Triangle";
}