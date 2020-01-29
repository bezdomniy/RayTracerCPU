#include "cylinder.h"

Cylinder::Cylinder() : Shape()
{
  this->minimum = -std::numeric_limits<float>::infinity();
  this->maximum = std::numeric_limits<float>::infinity();
  this->capped = false;
}

Cylinder::Cylinder(float minimum, float maximum, bool capped) : Shape()
{
  this->minimum = minimum;
  this->maximum = maximum;
  this->capped = capped;
}

Cylinder::~Cylinder() {}

void Cylinder::intersectRay(Ray &ray, std::vector<Geometry::Intersection<Shape>> &intersections)
{
  Ray transformedRay = transformRay(ray);

  float a = std::pow(transformedRay.direction.x, 2) +
            std::pow(transformedRay.direction.z, 2);

  if (std::abs(a) < Geometry::EPSILON)
    return;

  float b = (2 * transformedRay.origin.x * transformedRay.direction.x) +
            (2 * transformedRay.origin.z * transformedRay.direction.z);

  float c = std::pow(transformedRay.origin.x, 2) +
            std::pow(transformedRay.origin.z, 2) - 1;

  float discriminant = std::pow(b, 2) - 4 * a * c;

  if (discriminant < 0)
    return;

  float t1 = (-b - std::sqrt(discriminant)) / (2 * a);
  float t2 = (-b + std::sqrt(discriminant)) / (2 * a);

  float y1 = transformedRay.origin.y + t1 * transformedRay.direction.y;
  if (this->minimum < y1 and y1 < this->maximum)
    intersections.push_back(Geometry::Intersection<Shape>{t1, this});

  float y2 = transformedRay.origin.y + t2 * transformedRay.direction.y;
  if (this->minimum < y2 and y2 < this->maximum)
    intersections.push_back(Geometry::Intersection<Shape>{t2, this});

  intersectCaps(transformedRay, intersections);
}

glm::vec4 Cylinder::normalAt(glm::vec4 point)
{
  glm::vec4 objectPoint = this->inverseTransform * point;

  float dist = std::pow(objectPoint.x, 2) + std::pow(objectPoint.z, 2);
  glm::vec4 objectNormal;

  if (dist < 1 && objectPoint.y >= (this->maximum - Geometry::EPSILON))
    objectNormal = glm::vec4(0.f, 1.f, 0.f, 0.f);
  else if (dist < 1 && objectPoint.y <= (this->minimum + Geometry::EPSILON))
    objectNormal = glm::vec4(0.f, -1.f, 0.f, 0.f);
  else
    objectNormal = glm::vec4(objectPoint.x, 0.f, objectPoint.z, 0.f);

  glm::vec4 worldNormal = glm::transpose(this->inverseTransform) * objectNormal;
  worldNormal.w = 0.f;

  return glm::normalize(worldNormal);
}

std::string Cylinder::type() { return "Cylinder"; }

bool Cylinder::checkCap(Ray &ray, float t)
{
  float x = ray.origin.x + t * ray.direction.x;
  float z = ray.origin.z + t * ray.direction.z;

  return (std::pow(x, 2) + std::pow(z, 2)) <= 1;
}

void Cylinder::intersectCaps(Ray &ray, std::vector<Geometry::Intersection<Shape>> &intersections)
{
  std::vector<Geometry::Intersection<Shape>> ret;
  if (!this->capped || std::abs(ray.direction.y) < Geometry::EPSILON)
    return;

  float t1 = (this->minimum - ray.origin.y) / ray.direction.y;
  if (checkCap(ray, t1))
    intersections.push_back(Geometry::Intersection<Shape>{t1, this});

  float t2 = (this->maximum - ray.origin.y) / ray.direction.y;
  if (checkCap(ray, t2))
    intersections.push_back(Geometry::Intersection<Shape>{t2, this});
}
