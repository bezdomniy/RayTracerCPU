#pragma once

#include "camera.h"
#include "canvas.h"

#include "shape.h"

#include "geometry.h"
#include "pointLight.h"
#include "ray.h"
#include "world.h"

#ifdef __EMSCRIPTEN__
#ifdef WITH_THREADS
#include "concurrentqueue.h"
// #include <taskflow/taskflow.hpp>
#endif //WITH_THREADS
#else
// #include <execution>
#include "concurrentqueue.h"
#include <thread>
#endif

#include <algorithm>
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
#ifdef __EMSCRIPTEN__
  Renderer(std::shared_ptr<Camera> &c, uint8_t nWorkers);
  void renderPixel(World &world, std::pair<int, int> &pixel, uint8_t nWorkers);
#endif
  ~Renderer();

  bool isPathTracer = false;
  static const int RAY_BOUNCE_LIMIT = 4;
  static const int RAYS_PER_PIXEL = 1;
  // TODO static constexpr below
  int sqrtRaysPerPixel = (int)std::sqrt(RAYS_PER_PIXEL);
  double halfSubPixelSize = 1.0 / (double)sqrtRaysPerPixel / 2.0;

  Canvas canvas;

  glm::dvec3 rayColourAt(Ray &ray, World &world, std::vector<Geometry::Intersection<Shape>> &intersections, short remaining);
  glm::dvec3 pathColourAt(Ray &ray, World &world, std::vector<Geometry::Intersection<Shape>> &intersections, short remaining);
  glm::dvec3 reflectColour(Geometry::IntersectionParameters &comps, double reflective,
                           World &world, std::vector<Geometry::Intersection<Shape>> &intersections, short remaining);
  glm::dvec3 lighting(Shape *shape, std::shared_ptr<PointLight> &light,
                      glm::dvec4 &point, glm::dvec4 &eyev, glm::dvec4 &normalv,
                      bool &inShadow);
  glm::dvec3 lighting(Shape *shape, glm::dvec4 &point);
  glm::dvec3 shadeHit(Geometry::Intersection<Shape> *hit, Geometry::IntersectionParameters &comps, World &world, std::vector<Geometry::Intersection<Shape>> &intersections, short remaining);
  glm::dvec3 refractedColour(Geometry::IntersectionParameters &comps, double transparency,
                             World &world, std::vector<Geometry::Intersection<Shape>> &intersections, short remaining);
  bool isShadowed(glm::dvec4 &point, std::vector<Geometry::Intersection<Shape>> &intersections, World &world, std::shared_ptr<PointLight> &light);

  void render(World &world);
  void renderPixel(World &world, const std::pair<int, int> &pixel);
};
