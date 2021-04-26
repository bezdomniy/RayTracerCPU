#pragma once

#include "shape.h"

class PointLight : public Shape
{
private:
  friend class cereal::access;
  template <class Archive>
  void serialize(Archive &archive)
  {
    archive(cereal::virtual_base_class<Shape>(this), position, intensity);
  }

  // template <class Archive>
  // static void load_and_construct(Archive &archive, cereal::construct<PointLight> &construct)
  // {
  //   Vec4 position;
  //   Vec3 intensity;

  //   archive(position, intensity);
  //   construct(position, intensity);
  // }

  virtual Vec4 normalAt(const Vec4 &point) override;
  virtual Vec4 normalAt(const Vec4 &point, const Vec2 &uv) override;
  virtual void intersectRay(Ray &ray, std::vector<Geometry::Intersection<Shape>> &intersections) override;
  virtual std::pair<Vec4, Vec4> bounds() const override;
  virtual std::string type() override;

public:
  PointLight()
  {
  }
  PointLight(Vec4 position, Vec3 intensity);
  ~PointLight();

  Vec4 position;
  Vec3 intensity;
};

CEREAL_REGISTER_TYPE(PointLight);