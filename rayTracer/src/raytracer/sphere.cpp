#include "sphere.h"

Sphere::Sphere()
    : Shape()
{
}

Sphere::~Sphere() {}

void Sphere::intersectRay(Ray &ray, std::vector<Geometry::Intersection<Shape>> &intersections)
{
  Ray transformedRay = transformRay(ray);

  glm::dvec4 sphereToRay = transformedRay.origin - glm::dvec4(0.0, 0.0, 0.0, 1.0);
  double a = glm::dot(transformedRay.direction, transformedRay.direction);
  double b = 2 * glm::dot(transformedRay.direction, sphereToRay);
  double c =
      glm::dot(sphereToRay, sphereToRay) - 1;
  double discriminant = b * b - 4 * a * c;

  if (discriminant < 0)
    return;

  double t1 = (-b - std::sqrt(discriminant)) / (2 * a);
  double t2 = (-b + std::sqrt(discriminant)) / (2 * a);

  intersections.push_back(Geometry::Intersection<Shape>{t1, this});
  intersections.push_back(Geometry::Intersection<Shape>{t2, this});
}

glm::dvec4 Sphere::normalAt(const glm::dvec4 &point)
{
  // glm::dmat4 transformInverse(glm::affineInverse(this->transform));
  glm::dvec4 objectPoint = worldToObject(point);
  glm::dvec4 objectNormal = objectPoint - glm::dvec4(0.0, 0.0, 0.0, 1.0);

  return normalToWorld(objectNormal);
}

glm::dvec4 Sphere::normalAt(const glm::dvec4 &point, const glm::dvec2 &uv)
{
  return normalAt(point);
}

std::pair<glm::dvec4, glm::dvec4> Sphere::bounds()
{
  return std::pair<glm::dvec4, glm::dvec4>(glm::dvec4(-1., -1., -1., 1.), glm::dvec4(1., 1., 1., 1.));
}

std::string Sphere::type()
{
  return "Sphere";
}