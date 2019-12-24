#pragma once

#define GLM_ENABLE_EXPERIMENTAL
#define _CRT_SECURE_NO_WARNINGS

#include <glm/glm.hpp>
// #include <glm/gtx/intersect.hpp>
#include <vector>
// #include "shape.h"
// #include "intersection.h"
// #include "pointLight.h"
#include "ray.h"
// #include "material.h"
#include <algorithm>
#include <cmath>
#include <glm/gtc/matrix_inverse.hpp>
#include <limits>
#include <memory>

// class Shape;

namespace Geometry
{
static const float EPSILON = 0.00001f;
// static const float EPSILON = std::numeric_limits<float>::epsilon();
struct IntersectionParameters
{
  glm::vec4 point;
  glm::vec4 normalv;
  glm::vec4 eyev;
  glm::vec4 reflectv;
  glm::vec4 overPoint;
  glm::vec4 underPoint;
  float n1;
  float n2;
  bool inside;
};

template <typename T>
struct Intersection
{
  float t;
  T *shapePtr;
  std::unique_ptr<IntersectionParameters> comps;
};

template <typename T>
void getRefractiveIndexFromTo(std::vector<Intersection<T>> &intersections, Intersection<T> &hit)
{
  std::vector<T *> objects;

  for (auto &intersection : intersections)
  {
    if (&intersection == &hit)
    {
      if (objects.empty())
        intersection.comps->n1 = 1.f;
      else
        intersection.comps->n1 = objects.back()->material->refractiveIndex;
    }

    typename std::vector<T *>::iterator position = std::find(objects.begin(), objects.end(), intersection.shapePtr);
    if (position != objects.end())
      objects.erase(position);
    else
      objects.push_back(intersection.shapePtr);

    if (&intersection == &hit)
    {
      if (objects.empty())
        intersection.comps->n2 = 1.f;
      else
        intersection.comps->n2 = objects.back()->material->refractiveIndex;
      break;
    }
  }
}

template <typename T>
void getIntersectionParameters(Intersection<T> &intersection, Ray &ray, std::vector<Intersection<T>> &intersections)
{
  intersection.comps = std::make_unique<IntersectionParameters>();
  intersection.comps->point =
      ray.origin + glm::normalize(ray.direction) * intersection.t;
  intersection.comps->normalv =
      intersection.shapePtr->normalAt(intersection.comps->point);
  intersection.comps->eyev = -ray.direction;

  if (glm::dot(intersection.comps->normalv, intersection.comps->eyev) < 0)
  {
    intersection.comps->inside = true;
    intersection.comps->normalv = -intersection.comps->normalv;
  }
  else
  {
    intersection.comps->inside = false;
  }

  intersection.comps->reflectv = glm::reflect(ray.direction, intersection.comps->normalv);
  intersection.comps->overPoint =
      intersection.comps->point + intersection.comps->normalv * EPSILON;
  intersection.comps->underPoint =
      intersection.comps->point - intersection.comps->normalv * EPSILON;

  getRefractiveIndexFromTo<T>(intersections, intersection);
}

template <typename T>
bool compareIntersection(Intersection<T> &i1, Intersection<T> &i2)
{
  return (i1.t < i2.t);
}

template <typename T>
Intersection<T> *hit(std::vector<Intersection<T>> &intersections)
{
  int retIndex = -1;

  if (!intersections.empty())
  {
    for (int i = 0; i < intersections.size(); i++)
    {
      if ((retIndex == -1 && intersections.at(i).t > 0) || (intersections.at(i).t > 0 &&
                                                            intersections.at(i).t < intersections.at(retIndex).t))
      {
        retIndex = i;
      }
    }

    if (retIndex != -1 && intersections.at(retIndex).t > 0)
    {
      return &intersections.at(retIndex);
    }
  }
  return nullptr;
}

template <typename T>
float schlick(std::unique_ptr<IntersectionParameters> &comps)
{
  float cos = glm::dot(comps->eyev, comps->normalv);
  if (comps->n1 > comps->n2)
  {
    float n = comps->n1 / comps->n2;
    float sin2T = std::pow(n, 2) * (1.f - std::pow(cos, 2));
    if (sin2T > 1.f)
      return 1.f;

    float cosT = std::sqrt(1.f - sin2T);
    cos = cosT;
  }
  float r0 = std::pow((comps->n1 - comps->n2) / (comps->n1 + comps->n2), 2);
  return r0 + (1.f - r0) * std::pow(1.f - cos, 5);
}

template <typename T>
std::pair<float, float> checkAxis(float origin, float direction)
{
  float tmin_numerator = -1 - origin;
  float tmax_numerator = 1 - origin;

  std::pair<float, float> ret;

  if (std::abs(direction) >= EPSILON)
  {
    ret.first = tmin_numerator / direction;
    ret.second = tmax_numerator / direction;
  }
  else
  {
    ret.first = tmin_numerator * std::numeric_limits<float>::infinity();
    ret.second = tmax_numerator * std::numeric_limits<float>::infinity();
  }
  if (ret.first > ret.second)
    std::swap(ret.first, ret.second);

  return ret;
}

// ​ 	  ​if​ tmin > tmax ​then​ swap(tmin, tmax)
// ​
// ​ 	  ​return​ tmin, tmax
// ​ 	​end​ ​function

} // namespace Geometry