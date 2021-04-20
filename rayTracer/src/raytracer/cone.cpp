#include "cone.h"

Cone::Cone() : Shape()
{
  this->minimum = -std::numeric_limits<double>::infinity();
  this->maximum = std::numeric_limits<double>::infinity();
  this->capped = false;
}

Cone::Cone(double minimum, double maximum, bool capped) : Shape()
{
  this->minimum = minimum;
  this->maximum = maximum;
  this->capped = capped;
}

// TODO refactor to use unique_ptrs in object loader and make explicit copy constructors for all shapes
Cone::Cone(const Cone &c2)
{
  this->maximum = c2.maximum;
  this->minimum = c2.minimum;
  this->capped = c2.capped;
  this->inverseTransform = c2.inverseTransform;
  this->transform = c2.transform;
  this->parent = c2.parent;
}

Cone::~Cone() {}

void Cone::intersectRay(Ray &ray, std::vector<Geometry::Intersection<Shape>> &intersections)
{
  Ray transformedRay = transformRay(ray);
  bool singleIntersection = false;

  double a = std::pow(transformedRay.direction.x, 2) - std::pow(transformedRay.direction.y, 2) +
             std::pow(transformedRay.direction.z, 2);

  double b = (2 * transformedRay.origin.x * transformedRay.direction.x) -
             (2 * transformedRay.origin.y * transformedRay.direction.y) +
             (2 * transformedRay.origin.z * transformedRay.direction.z);

  if (std::abs(a) < Geometry::EPSILON)
  {
    if (std::abs(b) < Geometry::EPSILON)
    {
      return;
    }
    else
    {
      singleIntersection = false;
    }
  }
  double c = std::pow(transformedRay.origin.x, 2) - std::pow(transformedRay.origin.y, 2) +
             std::pow(transformedRay.origin.z, 2);

  if (singleIntersection)
  {
    double t = -c / (2 * b);
    intersections.push_back(Geometry::Intersection<Shape>{t, this});
    return;
  }

  double discriminant = std::pow(b, 2) - 4 * a * c;

  if (discriminant < 0)
    return;

  double t1 = (-b - std::sqrt(discriminant)) / (2 * a);
  double t2 = (-b + std::sqrt(discriminant)) / (2 * a);

  double y1 = transformedRay.origin.y + t1 * transformedRay.direction.y;
  if (this->minimum < y1 && y1 < this->maximum)
    intersections.push_back(Geometry::Intersection<Shape>{t1, this});

  double y2 = transformedRay.origin.y + t2 * transformedRay.direction.y;
  if (this->minimum < y2 && y2 < this->maximum)
    intersections.push_back(Geometry::Intersection<Shape>{t2, this});

  intersectCaps(transformedRay, intersections);
}

glm::dvec4 Cone::normalAt(const glm::dvec4 &point)
{
  glm::dvec4 objectPoint = worldToObject(point);

  double dist = std::pow(objectPoint.x, 2) + std::pow(objectPoint.z, 2);
  glm::dvec4 objectNormal;

  if (dist < 1 && objectPoint.y >= (this->maximum - Geometry::EPSILON))
    objectNormal = glm::dvec4(0.0, 1.0, 0.0, 0.0);
  else if (dist < 1 && objectPoint.y <= (this->minimum + Geometry::EPSILON))
    objectNormal = glm::dvec4(0.0, -1.0, 0.0, 0.0);
  else
  {
    double y = std::sqrt(std::pow(objectPoint.x, 2) + std::pow(objectPoint.z, 2));
    if (objectPoint.y - Geometry::EPSILON > 0)
    {
      y = -y;
    }
    objectNormal = glm::dvec4(objectPoint.x, y, objectPoint.z, 0.0);
  }

  return normalToWorld(objectNormal);
}

glm::dvec4 Cone::normalAt(const glm::dvec4 &point, const glm::dvec2 &uv)
{
  return normalAt(point);
}

std::pair<glm::dvec4, glm::dvec4> Cone::bounds() const
{
  return std::pair<glm::dvec4, glm::dvec4>(glm::dvec4(-1., this->minimum, -1., 1.), glm::dvec4(1., this->maximum, 1., 1.));
}

std::string Cone::type() { return "Cone"; }

bool Cone::checkCap(Ray &ray, double t, double y)
{
  double x = ray.origin.x + t * ray.direction.x;
  double z = ray.origin.z + t * ray.direction.z;

  return (std::pow(x, 2) + std::pow(z, 2)) <= std::fabs(y);
}

void Cone::intersectCaps(Ray &ray, std::vector<Geometry::Intersection<Shape>> &intersections)
{
  if (!this->capped || std::abs(ray.direction.y) < Geometry::EPSILON)
    return;

  double t1 = (this->minimum - ray.origin.y) / ray.direction.y;
  if (checkCap(ray, t1, this->minimum))
    intersections.push_back(Geometry::Intersection<Shape>{t1, this});

  double t2 = (this->maximum - ray.origin.y) / ray.direction.y;
  if (checkCap(ray, t2, this->maximum))
    intersections.push_back(Geometry::Intersection<Shape>{t2, this});
}
