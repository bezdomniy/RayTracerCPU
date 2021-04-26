#include "pointLight.h"

PointLight::PointLight(Vec4 position, Vec3 intensity) : Shape()
{
  this->position = position;
  this->intensity = intensity;
}

PointLight::~PointLight() {}

Vec4 PointLight::normalAt(const Vec4 &point) { return Vec4(); }

Vec4 PointLight::normalAt(const Vec4 &point, const Vec2 &uv)
{
  return normalAt(point);
}

void PointLight::intersectRay(Ray &ray, std::vector<Geometry::Intersection<Shape>> &intersections) { return; }

std::pair<Vec4, Vec4> PointLight::bounds() const
{
  return std::pair<Vec4, Vec4>();
}

std::string PointLight::type() { return "PointLight"; }
