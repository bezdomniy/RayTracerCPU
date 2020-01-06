#pragma once

#include "geometry.h"
#include "shape.h"
#include <glm/gtc/matrix_inverse.hpp>
#include <memory>

class Cylinder : public Shape {
public:
  Cylinder();
  Cylinder(float minimum, float maximum);
  ~Cylinder();

  virtual std::vector<Geometry::Intersection<Shape>>
  intersectRay(Ray &ray) override;
  virtual glm::vec4 normalAt(glm::vec4 point) override;
  virtual std::string type() override;

  float minimum;
  float maximum;

private:
};
