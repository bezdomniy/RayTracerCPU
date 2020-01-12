#pragma once

#include "camera.h"
#include "canvas.h"

#include "shape.h"

#include "geometry.h"
#include "pointLight.h"
#include "ray.h"
// #include "sphere.h"
#include "world.h"
// #include <algorithm>
// #ifdef __EMSCRIPTEN__
#include <pstl/algorithm>
#include <pstl/execution>
// #else
// #include <algorithm>
// #include <execution>
// #endif

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

  static const int RAY_BOUNCE_LIMIT = 5;
  static const int RAYS_PER_PIXEL = 1;
  Canvas canvas;

  glm::vec3 colourAt(Ray &ray, World &world, short remaining);
  glm::vec3 reflectColour(Geometry::Intersection<Shape> *hit, World &world,
                          short remaining);
  glm::vec3 lighting(Shape *shape, std::shared_ptr<PointLight> &light,
                     glm::vec4 &point, glm::vec4 &eyev, glm::vec4 &normalv,
                     bool &inShadow);
  glm::vec3 shadeHit(Geometry::Intersection<Shape> *hit, World &world,
                     short remaining);
  glm::vec3 refractedColour(Geometry::Intersection<Shape> *hit, World &world,
                            short remaining);
  bool isShadowed(glm::vec4 &point, World &world);

  void render(World &world);
  void renderPixel(World &world, std::pair<int, int> &pixel,
                   int sqrtRaysPerPixel, float halfSubPixelSize);
};
