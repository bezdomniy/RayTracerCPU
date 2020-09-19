#pragma once

#include "geometry.h"
#include "shape.h"
#include <glm/gtc/matrix_inverse.hpp>
#include <memory>
#include <cmath>

class Cone : public Shape
{
public:
  Cone();
  Cone(double minimum, double maximum, bool capped);

  Cone(const Cone &c2);
  ~Cone();

  virtual void intersectRay(Ray &ray, std::vector<Geometry::Intersection<Shape>> &intersections) override;
  virtual glm::dvec4 normalAt(const glm::dvec4 &point) override;
  virtual glm::dvec4 normalAt(const glm::dvec4 &point, const glm::dvec2 &uv) override;
  virtual std::pair<glm::dvec4, glm::dvec4> bounds() override;
  virtual std::string type() override;

  double minimum;
  double maximum;
  bool capped;

private:
  friend class cereal::access;
  template <class Archive>
  void serialize(Archive &archive)
  {
    archive(cereal::virtual_base_class<Shape>(this), minimum, maximum, capped);
  }

  bool checkCap(Ray &ray, double t, double y);

  void intersectCaps(Ray &ray, std::vector<Geometry::Intersection<Shape>> &intersections);
};

CEREAL_REGISTER_TYPE(Cone);