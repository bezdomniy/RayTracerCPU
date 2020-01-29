#pragma once

#include "shape.h"

class PointLight : public Shape
{
private:
  virtual glm::vec4 normalAt(glm::vec4 point) override;
  virtual void intersectRay(Ray& ray, std::vector<Geometry::Intersection<Shape>>& intersections) override;
  virtual std::string type() override;

public:
  PointLight(glm::vec4 position, glm::vec3 intensity);
  ~PointLight();

  glm::vec4 position;
  glm::vec3 intensity;
};
