#pragma once

#include "shape.h"

class PointLight : public Shape
{
private:
  virtual glm::vec4 normalAt(glm::vec4 point) override;
  virtual std::vector<Geometry::Intersection<Shape>> intersectRay(Ray &ray) override;
  virtual std::string type() override;

public:
  PointLight(unsigned int id, glm::vec4 position, glm::vec3 intensity);
  ~PointLight();

  int id;
  glm::vec4 position;
  glm::vec3 intensity;
};
