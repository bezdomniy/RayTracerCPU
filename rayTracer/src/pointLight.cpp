#include "pointLight.h"

PointLight::PointLight(unsigned int id, glm::vec4 position, glm::vec3 intensity) : Shape(id, position)
{
  this->position = position;
  this->intensity = intensity;
}

PointLight::~PointLight() {}

glm::vec4 PointLight::normalAt(glm::vec4 point)
{
  return glm::vec4();
}

std::vector<Geometry::Intersection<Shape>> PointLight::intersectRay(Ray &ray)
{
  return std::vector<Geometry::Intersection<Shape>>();
}

std::string PointLight::type()
{
  return "PointLight";
}
