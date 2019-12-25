#include "cube.h"

Cube::Cube()
    : Shape()
{
}

Cube::~Cube() {}

std::vector<Geometry::Intersection<Shape>> Cube::intersectRay(Ray &ray)
{
    Ray transformedRay = transformRay(ray);
    std::vector<Geometry::Intersection<Shape>> ret;

    std::pair<float, float> xtminmax = Geometry::checkAxis<float>(transformedRay.origin.x, transformedRay.direction.x);
    std::pair<float, float> ytminmax = Geometry::checkAxis<float>(transformedRay.origin.y, transformedRay.direction.y);
    std::pair<float, float> ztminmax = Geometry::checkAxis<float>(transformedRay.origin.z, transformedRay.direction.z);

    float tmin = std::max({xtminmax.first, ytminmax.first, ztminmax.first});
    float tmax = std::min({xtminmax.second, ytminmax.second, ztminmax.second});

    if (tmin > tmax)
        return ret;

    ret.push_back(Geometry::Intersection<Shape>{tmin, this});
    ret.push_back(Geometry::Intersection<Shape>{tmax, this});

    return ret;
}

glm::vec4 Cube::normalAt(glm::vec4 point)
{
    // glm::mat4 transformInverse(glm::affineInverse(this->transform));
    glm::vec4 objectPoint = this->inverseTransform * point;
    glm::vec4 objectNormal;

    float points[3] = {std::abs(objectPoint.x), std::abs(objectPoint.y), std::abs(objectPoint.z)};
    const int N = sizeof(points) / sizeof(float);
    int indexMaxC = std::distance(points, std::max_element(points, points + N));

    if (indexMaxC == 0)
        objectNormal = glm::normalize(glm::vec4(objectPoint.x, 0.f, 0.f, 0.f));
    else if (indexMaxC == 1)
        objectNormal = glm::normalize(glm::vec4(0.f, objectPoint.y, 0.f, 0.f));
    else
        objectNormal = glm::normalize(glm::vec4(0.f, 0.f, objectPoint.z, 0.f));

    glm::vec4 worldNormal = glm::transpose(this->inverseTransform) * objectNormal;
    worldNormal.w = 0.f;

    return glm::normalize(worldNormal);
}

std::string Cube::type()
{
    return "cube";
}