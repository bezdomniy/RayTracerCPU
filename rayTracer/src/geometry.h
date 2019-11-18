#pragma once

#define GLM_ENABLE_EXPERIMENTAL
#define _CRT_SECURE_NO_WARNINGS

#include <glm/glm.hpp>
#include <glm/gtx/intersect.hpp>
#include <vector>
#include "shape.h"
#include "pointLight.h"
#include "ray.h"
#include "material.h"
#include <algorithm>
#include <cmath>
#include <glm/gtc/matrix_inverse.hpp>
#include <limits>
#include <memory>

namespace Geometry
{
static const float EPSILON = 0.0001f;
struct IntersectionParameters
{
  glm::vec4 point;
  glm::vec4 normalv;
  glm::vec4 eyev;
  glm::vec4 overPoint;
  bool inside;
};

struct Intersection
{
  float t;
  std::shared_ptr<Shape> shapePtr;
  std::unique_ptr<IntersectionParameters> comps;
};

void getIntersectionParameters(Intersection &intersection, Ray &ray);
bool compareIntersection(Intersection &i1, Intersection &i2);
Intersection *hit(std::vector<Intersection> &intersections);

} // namespace Geometry