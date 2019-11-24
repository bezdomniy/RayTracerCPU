#pragma once

#include "geometry.h"
#include "pointLight.h"
#include "ray.h"
#include "shape.h"
#include "sphere.h"
#include <glm/glm.hpp>
#include <iostream>
#include <memory>
#include <vector>
#include <typeinfo>

class World
{
private:
  /* data */
public:
  World();
  ~World();

  void addShape(std::shared_ptr<Shape> &shape_ptr);
  void addLight(std::shared_ptr<PointLight> &light);

  bool isShadowed(glm::vec4 point);
  std::vector<Geometry::Intersection<Shape>> intersectRay(Ray &ray);

  std::vector<std::shared_ptr<Shape>> shapes;
  std::vector<std::shared_ptr<PointLight>> lights;
};
