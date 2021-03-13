#include "sphere.h"

Sphere::Sphere()
    : Shape()
{
}

Sphere::~Sphere() {}

void Sphere::intersectRay(Ray &ray, std::vector<Geometry::Intersection<Shape>> &intersections)
{
  Ray transformedRay = transformRay(ray);

  Vec4 sphereToRay = transformedRay.origin - Vec4(0.0, 0.0, 0.0, 1.0);
  Float a = glm::dot(transformedRay.direction, transformedRay.direction);
  Float b = 2 * glm::dot(transformedRay.direction, sphereToRay);
  Float c =
      glm::dot(sphereToRay, sphereToRay) - 1;
  Float discriminant = b * b - 4 * a * c;

  if (discriminant < 0)
    return;

  Float t1 = (-b - std::sqrt(discriminant)) / (2 * a);
  Float t2 = (-b + std::sqrt(discriminant)) / (2 * a);

  intersections.push_back(Geometry::Intersection<Shape>{t1, this});
  intersections.push_back(Geometry::Intersection<Shape>{t2, this});
}

Vec4 Sphere::normalAt(const Vec4 &point)
{
  // Mat4 transformInverse(glm::affineInverse(this->transform));
  Vec4 objectPoint = worldToObject(point);
  Vec4 objectNormal = objectPoint - Vec4(0.0, 0.0, 0.0, 1.0);

  return normalToWorld(objectNormal);
}

Vec4 Sphere::normalAt(const Vec4 &point, const Vec2 &uv)
{
  return normalAt(point);
}

std::pair<Vec4, Vec4> Sphere::bounds()
{
  return std::pair<Vec4, Vec4>(Vec4(-1., -1., -1., 1.), Vec4(1., 1., 1., 1.));
}

std::string Sphere::type()
{
  return "Sphere";
}