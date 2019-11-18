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

class World
{
private:
  /* data */
public:
  World();
  ~World();

  void addShape(Shape &shape);
  void addSphere(Sphere &sphere);
  void addLight(std::shared_ptr<PointLight> &light);

  bool isShadowed(glm::vec4 point);
  std::vector<Geometry::Intersection> intersectRay(Ray &ray);

  std::vector<std::shared_ptr<Sphere>> spheres;
  std::vector<std::shared_ptr<PointLight>> lights;
};
