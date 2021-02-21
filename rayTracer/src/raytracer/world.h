#pragma once

#include "geometry.h"
#include "pointLight.h"
#include "ray.h"
#include "shape.h"
#include "sphere.h"
#include <glm/glm.hpp>
#include <iostream>
#include <memory>
#include <typeinfo>
#include <vector>

#include <cereal/types/vector.hpp>
// #include <cereal/archives/binary.hpp>
#include <cereal/types/memory.hpp>
#include "serialisation.h"

class World
{
private:
  friend class cereal::access;
  template <class Archive>
  void serialize(Archive &archive)
  {
    archive(shapes, lights);
  }

public:
  World();
  ~World();

  void addShape(std::shared_ptr<Shape> &shape_ptr);
  void addLight(std::shared_ptr<PointLight> &light);

  bool isShadowed(glm::dvec4 point);
  void intersectRay(Ray &ray, std::vector<Geometry::Intersection<Shape>> &ret);
  bool intersectRayShadow(Ray &ray, std::vector<Geometry::Intersection<Shape>> &intersectionBuffer, double distance);

  std::vector<std::shared_ptr<Shape>> shapes;
  std::vector<std::shared_ptr<PointLight>> lights;
};
