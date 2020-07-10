#pragma once

#include "geometry.h"
#include "objectLoader.h"
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
#include <cereal/archives/binary.hpp>
#include <cereal/types/memory.hpp>

class World
{
private:
  friend class cereal::access;
  template <class Archive>
  void serialize(Archive &archive)
  {
    archive(CEREAL_NVP(this->shapes), CEREAL_NVP(this->lights));
  }
  // TODO remove this as member, make static
  std::unique_ptr<ObjectLoader> objectLoader;

public:
  World();
  ~World();

  std::shared_ptr<Camera> loadFromFile(const std::string &fileName);

  void addShape(std::shared_ptr<Shape> &shape_ptr);
  void addLight(std::shared_ptr<PointLight> &light);

  bool isShadowed(glm::dvec4 point);
  std::vector<Geometry::Intersection<Shape>> intersectRay(Ray &ray);
  std::vector<Geometry::Intersection<Shape>> intersectRayShadow(Ray &ray);

  std::vector<std::shared_ptr<Shape>> shapes;
  std::vector<std::shared_ptr<PointLight>> lights;
};
