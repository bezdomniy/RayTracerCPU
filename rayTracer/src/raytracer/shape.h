#pragma once

#include "types.h"
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
  virtual Vec4 normalAt(const Vec4 &point) = 0;
  virtual Vec4 normalAt(const Vec4 &point, const Vec2 &uv) = 0;
  virtual std::string type() = 0;
  virtual std::pair<Vec4, Vec4> bounds() = 0;

  Mat4 transform;
  Mat4 inverseTransform;

  Vec4 boundsCentroid();
  std::shared_ptr<Shape> parent = nullptr;
  std::shared_ptr<Material> material;
  // bool materialSet = false; //TODO get rid of bool

  // std::pair<Vec4,Vec4> boundingMinMax;

  virtual void setMaterial(std::shared_ptr<Material> &mat);
  Vec3 patternAt(const Vec4 &point);
  void multiplyTransform(Mat4 &transform);
  void calculateInverseTranform();

  Vec4 worldToObject(const Vec4 &point);
  Vec4 normalToWorld(const Vec4 &normal);

  Ray transformRay(Ray &ray);
  void transformRayInPlace(Ray &ray);
};
