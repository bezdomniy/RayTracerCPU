#pragma once

#include "shape.h"

class PointLight : public Shape
{
private:
  virtual glm::dvec4 normalAt(glm::dvec4 point) override;
  virtual void intersectRay(Ray& ray, std::vector<Geometry::Intersection<Shape>>& intersections) override;
  virtual std::string type() override;

public:
  PointLight(glm::dvec4 position, glm::dvec3 intensity);
  ~PointLight();

  glm::dvec4 position;
  glm::dvec3 intensity;
};
