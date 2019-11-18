#include "sphere.h"

Sphere::Sphere(unsigned int id, glm::vec4 position, float radius)
    : Shape(id, position)
{
  this->radius = radius;
}

Sphere::~Sphere() {}

std::vector<Geometry::Intersection<Shape>> Sphere::intersectRay(Ray &ray)
{
  std::vector<Geometry::Intersection<Shape>> ret;

  Ray transformedRay = transformRay(ray);

  // std::cout << transformedRay <<std::endl;

  glm::vec4 sphereToRay = transformedRay.origin - this->position;
  float a = glm::dot(transformedRay.direction, transformedRay.direction);
  float b = 2 * glm::dot(transformedRay.direction, sphereToRay);
  float c =
      glm::dot(sphereToRay, sphereToRay) - (this->radius * this->radius);
  float discriminant = b * b - 4 * a * c;

  if (discriminant < 0)
    return ret;

  float t1 = (-b - std::sqrt(discriminant)) / (2 * a);
  float t2 = (-b + std::sqrt(discriminant)) / (2 * a);

  ret.push_back(Geometry::Intersection<Shape>{t1, std::make_shared<Sphere>(*this)});
  ret.push_back(Geometry::Intersection<Shape>{t2, std::make_shared<Sphere>(*this)});

  return ret;
}