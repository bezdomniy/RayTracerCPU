#include "smoothTriangle.h"

SmoothTriangle::SmoothTriangle(Vec3 p1, Vec3 p2, Vec3 p3, Vec3 n1, Vec3 n2, Vec3 n3) : Triangle(p1, p2, p3)
{
    this->inverseTransform[3] = Vec4(n1, 0.0);
    glm::column(this->inverseTransform, 3) = Vec4(n2, 0.0);

    // this->n2 = n2;
    this->n3 = n3;
}

SmoothTriangle::~SmoothTriangle()
{
}

Vec4 SmoothTriangle::normalAt(const Vec4 &point, const Vec2 &uv)
{
    Float one = 1.0;
    return normalToWorld(Vec4(Vec3(glm::column(this->inverseTransform, 3)) * uv.x + this->n3 * uv.y + Vec3(this->inverseTransform[3]) * (one - uv.x - uv.y), 0.));
}