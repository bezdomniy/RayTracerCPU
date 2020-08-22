#pragma once

#include <glm/glm.hpp>
#include <ostream>

class Ray {
public:
  Ray();
  Ray(glm::dvec4, glm::dvec4);
  ~Ray();

  glm::dvec4 position(double);

  glm::dvec4 origin;
  glm::dvec4 direction;

  Ray transform(glm::dmat4 &m);
  Ray transform(glm::dmat4 &m, glm::dvec4 newOrigin);
  void transformInPlace(glm::dmat4 &m);

private:
};

std::ostream &operator<<(std::ostream &out, Ray const &r);
