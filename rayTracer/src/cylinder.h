#pragma once

#include "geometry.h"
#include "shape.h"
#include <glm/gtc/matrix_inverse.hpp>
#include <memory>

class Cylinder : public Shape
{
public:
  Cylinder();
  Cylinder(float minimum, float maximum, bool capped);
  ~Cylinder();

  virtual void intersectRay(Ray &ray, std::vector<Geometry::Intersection<Shape>> &intersections) override;
  virtual glm::vec4 normalAt(glm::vec4 point) override;
  virtual std::string type() override;

  float minimum;
  float maximum;
  bool capped;

private:
  bool checkCap(Ray &ray, float t);

  void intersectCaps(Ray &ray, std::vector<Geometry::Intersection<Shape>> &intersections);
};
