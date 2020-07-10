#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_inverse.hpp>
#include <unordered_map>
#include <vector>

#include <cereal/archives/binary.hpp>
#include <cereal/types/memory.hpp>

#include "geometry.h"
#include "material.h"
#include "ray.h"

class Shape
{
private:
  friend class cereal::access;
  template <class Archive>
  void serialize(Archive &archive)
  {
    // archive(CEREAL_NVP(shapes), CEREAL_NVP(lights));
  }

public:
  Shape();
  virtual ~Shape() = 0;

  std::shared_ptr<Shape> parent = nullptr;

  bool materialSet = false;

  virtual void intersectRay(Ray &ray, std::vector<Geometry::Intersection<Shape>> &intersections) = 0;
  virtual glm::dvec4 normalAt(glm::dvec4 point) = 0;
  virtual glm::dvec4 normalAt(glm::dvec4 point, glm::dvec2 uv) = 0;
  virtual std::string type() = 0;
  virtual std::pair<glm::dvec4, glm::dvec4> bounds() = 0;

  glm::dvec4 boundsCentroid();

  glm::dmat4 transform;
  glm::dmat4 inverseTransform;

  // std::pair<glm::dvec4,glm::dvec4> boundingMinMax;

  std::shared_ptr<Material> material;
  virtual void setMaterial(std::shared_ptr<Material> &mat);
  glm::dvec3 patternAt(glm::dvec4 point);
  void multiplyTransform(glm::dmat4 &transform);
  void calculateInverseTranform();

  glm::dvec4 worldToObject(glm::dvec4 point);
  glm::dvec4 normalToWorld(glm::dvec4 normal);

  Ray transformRay(Ray &ray);
  void transformRayInPlace(Ray &ray);
};
