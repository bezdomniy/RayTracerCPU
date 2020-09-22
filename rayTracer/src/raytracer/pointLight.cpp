#include "pointLight.h"

PointLight::PointLight(glm::dvec4 position, glm::dvec3 intensity) : Shape()
{
  this->position = position;
  this->intensity = intensity;
}

PointLight::~PointLight() {}

glm::dvec4 PointLight::normalAt(const glm::dvec4 &point) { return glm::dvec4(); }

glm::dvec4 PointLight::normalAt(const glm::dvec4 &point, const glm::dvec2 &uv)
{
  return normalAt(point);
}

void PointLight::intersectRay(Ray &ray, std::vector<Geometry::Intersection<Shape>> &intersections) { return; }

std::pair<glm::dvec4, glm::dvec4> PointLight::bounds()
{
  return std::pair<glm::dvec4, glm::dvec4>();
}

std::string PointLight::type() { return "PointLight"; }
