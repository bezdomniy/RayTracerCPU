#pragma once

#include "geometry.h"
#include "shape.h"
#include <glm/gtc/matrix_inverse.hpp>
#include <memory>

class Cylinder : public Shape
{
public:
  Cylinder();
  Cylinder(double minimum, double maximum, bool capped);

  Cylinder(const Cylinder &c2);
  ~Cylinder();

  virtual void intersectRay(Ray &ray, std::vector<Geometry::Intersection<Shape>> &intersections) override;
  virtual glm::dvec4 normalAt(glm::dvec4 point) override;
  virtual glm::dvec4 normalAt(glm::dvec4 point, glm::dvec2 uv) override;
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
    archive(cereal::base_class<Shape>(this), minimum, maximum, capped);
  }

  bool checkCap(Ray &ray, double t);

  void intersectCaps(Ray &ray, std::vector<Geometry::Intersection<Shape>> &intersections);
};

CEREAL_REGISTER_TYPE(Cylinder);