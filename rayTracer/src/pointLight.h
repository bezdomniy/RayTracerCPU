#pragma once

#include "shape.h"

class PointLight : public Shape
{
private:
  friend class cereal::access;
  template <class Archive>
  void serialize(Archive &archive)
  {
    archive(cereal::base_class<Shape>(this), position, intensity);
  }

  template <class Archive>
  static void load_and_construct(Archive &archive, cereal::construct<PointLight> &construct)
  {
    glm::dvec4 position;
    glm::dvec3 intensity;

    archive(position, intensity);
    construct(position, intensity);
  }

  virtual glm::dvec4 normalAt(glm::dvec4 point) override;
  virtual glm::dvec4 normalAt(glm::dvec4 point, glm::dvec2 uv) override;
  virtual void intersectRay(Ray &ray, std::vector<Geometry::Intersection<Shape>> &intersections) override;
  virtual std::pair<glm::dvec4, glm::dvec4> bounds() override;
  virtual std::string type() override;

public:
  PointLight(glm::dvec4 position, glm::dvec3 intensity);
  ~PointLight();

  glm::dvec4 position;
  glm::dvec3 intensity;
};

CEREAL_REGISTER_TYPE(PointLight);