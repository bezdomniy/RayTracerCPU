#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_inverse.hpp>
#include <vector>

#include "ray.h"
#include "material.h"
#include "geometry.h"

class Shape
{
private:
  /* data */
public:
  Shape(unsigned int id, glm::vec4 position);
  virtual ~Shape();

  virtual std::vector<Geometry::Intersection<Shape>> intersectRay(Ray &ray) = 0;
  virtual glm::vec4 normalAt(glm::vec4 point) = 0;

  glm::mat4 transform;
  unsigned int id;
  glm::vec4 position;

  std::shared_ptr<Material> material;
  void setMaterial(std::shared_ptr<Material> &mat);
  glm::vec3 patternAt(glm::vec4 point);

  Ray transformRay(Ray &ray);
};
