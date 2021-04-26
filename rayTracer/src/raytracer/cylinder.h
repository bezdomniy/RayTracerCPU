#pragma once

#include "geometry.h"
#include "shape.h"
#include <glm/gtc/matrix_inverse.hpp>
#include <memory>

class Cylinder : public Shape
{
public:
  Cylinder();
  Cylinder(Float minimum, Float maximum, bool capped);

  Cylinder(const Cylinder &c2);
  ~Cylinder();

  virtual void intersectRay(Ray &ray, std::vector<Geometry::Intersection<Shape>> &intersections) override;
  virtual Vec4 normalAt(const Vec4 &point) override;
  virtual Vec4 normalAt(const Vec4 &point, const Vec2 &uv) override;
  virtual std::pair<Vec4, Vec4> bounds() const override;
  virtual std::string type() override;

  Float minimum;
  Float maximum;
  bool capped;

private:
  friend class cereal::access;
  template <class Archive>
  void serialize(Archive &archive)
  {
    archive(cereal::virtual_base_class<Shape>(this), minimum, maximum, capped);
  }

  bool checkCap(Ray &ray, Float t);

  void intersectCaps(Ray &ray, std::vector<Geometry::Intersection<Shape>> &intersections);
};

CEREAL_REGISTER_TYPE(Cylinder);