#pragma once

#include "material.h"
#include "shape.h"
#include "geometry.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_inverse.hpp>
#include <memory>

class Sphere : public Shape
{
public:
  Sphere(unsigned int id, glm::vec4 position, float radius);
  ~Sphere();

  float radius;
  std::vector<Geometry::Intersection> intersectRay(Ray &ray);

private:
};
