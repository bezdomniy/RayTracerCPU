#include "cylinder.h"

Cylinder::Cylinder() : Shape()
{
  this->minimum = -std::numeric_limits<Float>::infinity();
  this->maximum = std::numeric_limits<Float>::infinity();
  this->capped = false;
}

Cylinder::Cylinder(Float minimum, Float maximum, bool capped) : Shape()
{
  this->minimum = minimum;
  this->maximum = maximum;
  this->capped = capped;
}

// TODO refactor to use unique_ptrs in object loader and make explicit copy constructors for all shapes
Cylinder::Cylinder(const Cylinder &c2)
{
  this->maximum = c2.maximum;
  this->minimum = c2.minimum;
  this->capped = c2.capped;
  this->inverseTransform = c2.inverseTransform;
  this->transform = c2.transform;
  this->parent = c2.parent;
}

Cylinder::~Cylinder() {}

void Cylinder::intersectRay(Ray &ray, std::vector<Geometry::Intersection<Shape>> &intersections)
{
  Ray transformedRay = transformRay(ray);

  Float a = std::pow(transformedRay.direction.x, (Float)2) +
            std::pow(transformedRay.direction.z, (Float)2);

  if (std::abs(a) < Geometry::EPSILON)
    return;

  Float b = ((Float)2 * transformedRay.origin.x * transformedRay.direction.x) +
            ((Float)2 * transformedRay.origin.z * transformedRay.direction.z);

  Float c = std::pow(transformedRay.origin.x, (Float)2) +
            std::pow(transformedRay.origin.z, (Float)2) - 1;

  Float discriminant = std::pow(b, (Float)2) - (Float)4 * a * c;

  if (discriminant < 0)
    return;

  Float t1 = (-b - std::sqrt(discriminant)) / ((Float)2 * a);
  Float t2 = (-b + std::sqrt(discriminant)) / ((Float)2 * a);

  Float y1 = transformedRay.origin.y + t1 * transformedRay.direction.y;
  if (this->minimum < y1 && y1 < this->maximum)
    intersections.push_back(Geometry::Intersection<Shape>{t1, this});

  Float y2 = transformedRay.origin.y + t2 * transformedRay.direction.y;
  if (this->minimum < y2 && y2 < this->maximum)
    intersections.push_back(Geometry::Intersection<Shape>{t2, this});

  intersectCaps(transformedRay, intersections);
}

Vec4 Cylinder::normalAt(const Vec4 &point)
{
  Vec4 objectPoint = worldToObject(point);

  Float dist = std::pow(objectPoint.x, (Float)2) + std::pow(objectPoint.z, (Float)2);
  Vec4 objectNormal;

  if (dist < 1 && objectPoint.y >= (this->maximum - Geometry::EPSILON))
    objectNormal = Vec4((Float)0.0, (Float)1.0, (Float)0.0, (Float)0.0);
  else if (dist < 1 && objectPoint.y <= (this->minimum + Geometry::EPSILON))
    objectNormal = Vec4((Float)0.0, (Float)-1.0, (Float)0.0, (Float)0.0);
  else
    objectNormal = Vec4(objectPoint.x, (Float)0.0, objectPoint.z, (Float)0.0);

  return normalToWorld(objectNormal);
}

Vec4 Cylinder::normalAt(const Vec4 &point, const Vec2 &uv)
{
  return normalAt(point);
}

std::pair<Vec4, Vec4> Cylinder::bounds()
{
  return std::pair<Vec4, Vec4>(Vec4(-1., this->minimum, (Float)-1., (Float)1.), Vec4(1., this->maximum, (Float)1., (Float)1.));
}

std::string Cylinder::type() { return "Cylinder"; }

bool Cylinder::checkCap(Ray &ray, Float t)
{
  Float x = ray.origin.x + t * ray.direction.x;
  Float z = ray.origin.z + t * ray.direction.z;

  return (std::pow(x, (Float)2) + std::pow(z, (Float)2)) <= 1;
}

void Cylinder::intersectCaps(Ray &ray, std::vector<Geometry::Intersection<Shape>> &intersections)
{
  if (!this->capped || std::abs(ray.direction.y) < Geometry::EPSILON)
    return;

  Float t1 = (this->minimum - ray.origin.y) / ray.direction.y;
  if (checkCap(ray, t1))
    intersections.push_back(Geometry::Intersection<Shape>{t1, this});

  Float t2 = (this->maximum - ray.origin.y) / ray.direction.y;
  if (checkCap(ray, t2))
    intersections.push_back(Geometry::Intersection<Shape>{t2, this});
}
