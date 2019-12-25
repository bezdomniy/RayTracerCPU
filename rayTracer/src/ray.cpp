#include "ray.h"

Ray::Ray() {}

Ray::Ray(glm::vec4 origin, glm::vec4 direction)
{
  this->origin = origin;
  this->direction = direction;
}

Ray::~Ray() {}

std::ostream &operator<<(std::ostream &out, Ray const &r)
{
  out << "Ray: origin: " << r.origin.x << " " << r.origin.y << " "
      << r.origin.z;
  out << ", direction: " << r.direction.x << " " << r.direction.y << " "
      << r.direction.z;
  return out;
}

glm::vec4 Ray::position(float t)
{
  return origin + direction * t;
}

Ray Ray::transform(glm::mat4 &m)
{
  return Ray(m * this->origin, m * this->direction);
}

Ray Ray::transform(glm::mat4 &m, glm::vec4 newOrigin)
{
  return Ray(newOrigin, m * this->direction);
}

void Ray::transformInPlace(glm::mat4 &m)
{
  this->origin = m * this->origin;
  this->direction = m * this->direction;
}