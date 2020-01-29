#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_inverse.hpp>
#include <vector>
#include <unordered_map>

#include "ray.h"
#include "material.h"
#include "geometry.h"

class Shape
{
private:
  /* data */
public:
  Shape();
  virtual ~Shape();

  virtual void intersectRay(Ray& ray, std::vector<Geometry::Intersection<Shape>>& intersections) = 0;
  virtual glm::vec4 normalAt(glm::vec4 point) = 0;
  virtual std::string type() = 0;

  glm::mat4 transform;
  glm::mat4 inverseTransform;

  std::shared_ptr<Material> material;
  void setMaterial(std::shared_ptr<Material> &mat);
  glm::vec3 patternAt(glm::vec4 point);
  void multiplyTransform(glm::mat4 &transform);
  void calculateInverseTranform();

  Ray transformRay(Ray &ray);
  void transformRayInPlace(Ray &ray);
};
