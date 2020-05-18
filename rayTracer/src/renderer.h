#pragma once

#include "camera.h"
#include "canvas.h"

#include "shape.h"

#include "geometry.h"
#include "pointLight.h"
#include "ray.h"
#include "world.h"
#include <algorithm>
#include <execution>

#include <functional>
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <iostream>
#include <memory>
#include <random>

class Renderer
{
private:
  std::shared_ptr<Camera> camera;

public:
  Renderer();
  Renderer(std::shared_ptr<Camera> &c);
  ~Renderer();

  static const int RAY_BOUNCE_LIMIT = 3;
  static const int RAYS_PER_PIXEL = 1;
  Canvas canvas;

  glm::dvec3 colourAt(Ray &ray, World &world, short remaining);
  glm::dvec3 reflectColour(Geometry::Intersection<Shape> *hit, World &world,
                           short remaining);
  glm::dvec3 lighting(Shape *shape, std::shared_ptr<PointLight> &light,
                      glm::dvec4 &point, glm::dvec4 &eyev, glm::dvec4 &normalv,
                      bool &inShadow);
  glm::dvec3 shadeHit(Geometry::Intersection<Shape> *hit, World &world,
                      short remaining);
  glm::dvec3 refractedColour(Geometry::Intersection<Shape> *hit, World &world,
                             short remaining);
  bool isShadowed(glm::dvec4 &point, World &world, std::shared_ptr<PointLight> &light);

  void render(World &world);
  void renderPixel(World &world, std::pair<int, int> &pixel,
                   int sqrtRaysPerPixel, double halfSubPixelSize);
};
