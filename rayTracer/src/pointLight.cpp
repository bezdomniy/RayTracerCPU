#include "pointLight.h"

PointLight::PointLight(glm::vec4 position, glm::vec3 intensity) : Shape() {
  this->position = position;
  this->intensity = intensity;
}

PointLight::~PointLight() {}

glm::vec4 PointLight::normalAt(glm::vec4 point) { return glm::vec4(); }

void PointLight::intersectRay(Ray& ray, std::vector<Geometry::Intersection<Shape>>& intersections) { return; }

std::string PointLight::type() { return "PointLight"; }
