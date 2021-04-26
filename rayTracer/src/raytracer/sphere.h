#pragma once

// #include "material.h"
#include "shape.h"
#include "geometry.h"
// #include "types.h"
#include <glm/gtc/matrix_inverse.hpp>
#include <memory>

class Sphere : public Shape
{
private:
  friend class cereal::access;
  template <class Archive>
  void serialize(Archive &archive)
  {
    archive(cereal::virtual_base_class<Shape>(this));
  }

public:
  Sphere();
  ~Sphere();

  virtual void intersectRay(Ray &ray, std::vector<Geometry::Intersection<Shape>> &intersections) override;
  virtual Vec4 normalAt(const Vec4 &point) override;
  virtual Vec4 normalAt(const Vec4 &point, const Vec2 &uv) override;
  virtual std::pair<Vec4, Vec4> bounds() const override;
  virtual std::string type() override;
};

CEREAL_REGISTER_TYPE(Sphere);