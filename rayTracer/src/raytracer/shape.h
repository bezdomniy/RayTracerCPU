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
    archive(parent, inverseTransform, material);
  }

public:
  Shape();
  virtual ~Shape() = 0;

  virtual void intersectRay(Ray &ray, std::vector<Geometry::Intersection<Shape>> &intersections) = 0;
  virtual Vec4 normalAt(const Vec4 &point) = 0;
  virtual Vec4 normalAt(const Vec4 &point, const Vec2 &uv) = 0;
  virtual std::string type() = 0;

  //    TODO change to return const reference (const std::pair & )
  virtual std::pair<Vec4, Vec4> bounds() const = 0;

  // TODO: maybe make these into unique_ptrs so as you don't waste memory for shapes that dont use them
  // glm::dmat4 transform;
  Mat4 inverseTransform;
  std::shared_ptr<Shape> parent = nullptr;
  std::shared_ptr<Material> material;

  Vec4 boundsCentroid() const;

  // bool materialSet = false; //TODO get rid of bool

  // std::pair<Vec4,Vec4> boundingMinMax;

  std::shared_ptr<Material> &getMaterial();
  virtual void setMaterial(std::shared_ptr<Material> &mat);
  Vec3 patternAt(const Vec4 &point);
  // void multiplyTransform(glm::dmat4 &transform);
  void calculateInverseTranform(Mat4 &transform);
  void calculateInverseTranform(std::vector<Mat4> &transforms);

  Vec4 worldToObject(const Vec4 &point);
  Vec4 normalToWorld(const Vec4 &normal);

  Ray transformRay(Ray &ray);
  void transformRayInPlace(Ray &ray);
};
