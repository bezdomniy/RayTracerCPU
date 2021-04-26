#include "cube.h"

Cube::Cube()
    : Shape()
{
}

Cube::~Cube() {}

void Cube::intersectRay(Ray &ray, std::vector<Geometry::Intersection<Shape>> &intersections)
{
    Ray transformedRay = transformRay(ray);

    Float tmin = -std::numeric_limits<Float>::infinity();
    Float tmax = std::numeric_limits<Float>::infinity();

    for (int a = 0; a < 3; a++)
    {
        auto invD = 1.0 / transformedRay.direction[a];
        auto t0 = (-1.0 - transformedRay.origin[a]) * invD;
        auto t1 = (1.0 - transformedRay.origin[a]) * invD;
        if (invD < 0.0)
            std::swap(t0, t1);
        tmin = t0 > tmin ? t0 : tmin;
        tmax = t1 < tmax ? t1 : tmax;
        if (tmax <= tmin)
            return;
    }

    intersections.push_back(Geometry::Intersection<Shape>{tmin, this});
    intersections.push_back(Geometry::Intersection<Shape>{tmax, this});
}

Vec4 Cube::normalAt(const Vec4 &point)
{
    // Mat4 transformInverse(glm::affineInverse(this->transform));
    Vec4 objectPoint = worldToObject(point);
    Vec4 objectNormal;

    Float points[3] = {std::abs(objectPoint.x), std::abs(objectPoint.y), std::abs(objectPoint.z)};
    const int N = sizeof(points) / sizeof(Float);
    int indexMaxC = std::distance(points, std::max_element(points, points + N));

    if (indexMaxC == 0)
        objectNormal = glm::normalize(Vec4(objectPoint.x, 0.0, 0.0, 0.0));
    else if (indexMaxC == 1)
        objectNormal = glm::normalize(Vec4(0.0, objectPoint.y, 0.0, 0.0));
    else
        objectNormal = glm::normalize(Vec4(0.0, 0.0, objectPoint.z, 0.0));

    return normalToWorld(objectNormal);
}

Vec4 Cube::normalAt(const Vec4 &point, const Vec2 &uv)
{
    return normalAt(point);
}

std::pair<Vec4, Vec4> Cube::bounds() const
{
    return std::pair<Vec4, Vec4>(Vec4(-1., -1., -1., 1.), Vec4(1., 1., 1., 1.));
}

std::string Cube::type()
{
    return "Cube";
}
