#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_inverse.hpp>
#include <unordered_map>
#include <vector>

// #include <cereal/archives/binary.hpp>
#include <cereal/types/memory.hpp>

#include "geometry.h"
#include "material.h"
#include "ray.h"

#include "serialisation.h"

class Shape
{
private:
  friend class cereal::access;
  template <class Archive>
  void serialize(Archive &archive)
  {
    archive(parent, transform, inverseTransform, material);
  }

public:
  Shape();
  virtual ~Shape() = 0;

  virtual void intersectRay(Ray &ray, std::vector<Geometry::Intersection<Shape>> &intersections) = 0;
  virtual glm::dvec4 normalAt(const glm::dvec4 &point) = 0;
  virtual glm::dvec4 normalAt(const glm::dvec4 &point, const glm::dvec2 &uv) = 0;
  virtual std::string type() = 0;

  //    TODO change to return const reference (const std::pair & )
  virtual std::pair<glm::dvec4, glm::dvec4> bounds() const = 0;

  // TODO: maybe make these into unique_ptrs so as you don't waste memory for shapes that dont use them
  glm::dmat4 transform;
  glm::dmat4 inverseTransform;

  glm::dvec4 boundsCentroid() const;
  std::shared_ptr<Shape> parent = nullptr;
  std::shared_ptr<Material> material;
  // bool materialSet = false; //TODO get rid of bool

  // std::pair<glm::dvec4,glm::dvec4> boundingMinMax;

  std::shared_ptr<Material> &getMaterial();
  virtual void setMaterial(std::shared_ptr<Material> &mat);
  glm::dvec3 patternAt(const glm::dvec4 &point);
  void multiplyTransform(glm::dmat4 &transform);
  void calculateInverseTranform();

  glm::dvec4 worldToObject(const glm::dvec4 &point);
  glm::dvec4 normalToWorld(const glm::dvec4 &normal);

  Ray transformRay(Ray &ray);
  void transformRayInPlace(Ray &ray);
};
