#pragma once

// #include "material.h"
#include "shape.h"
#include "geometry.h"
// #include <glm/glm.hpp>
#include <glm/gtc/matrix_inverse.hpp>
#include <memory>

class Sphere : public Shape
{
public:
  Sphere();
  ~Sphere();

  virtual std::vector<Geometry::Intersection<Shape>> intersectRay(Ray &ray) override;
  virtual glm::vec4 normalAt(glm::vec4 point) override;
  virtual std::string type() override;

private:
};
