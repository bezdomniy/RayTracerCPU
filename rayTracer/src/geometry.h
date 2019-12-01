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
  std::shared_ptr<T> shapePtr;
  std::unique_ptr<IntersectionParameters> comps;
};

template <typename T>
void getIntersectionParameters(Intersection<T> &intersection, Ray &ray, std::vector<Intersection<T>> &intersections)
{
  intersection.comps = std::make_unique<IntersectionParameters>();
  intersection.comps->point =
      ray.origin + glm::normalize(ray.direction) * intersection.t;
  intersection.comps->normalv =
      intersection.shapePtr->normalAt(intersection.comps->point);
  intersection.comps->eyev = -ray.direction;
  intersection.comps->reflectv = glm::reflect(ray.direction, intersection.comps->normalv);
  intersection.comps->overPoint =
      intersection.comps->point + intersection.comps->normalv * EPSILON;
  intersection.comps->underPoint =
      intersection.comps->point - intersection.comps->normalv * EPSILON;

  if (glm::dot(intersection.comps->normalv, intersection.comps->eyev) < 0)
  {
    intersection.comps->inside = true;
    intersection.comps->normalv = -intersection.comps->normalv;
  }
  else
  {
    intersection.comps->inside = false;
  }

  getRefractiveIndexFromTo<T>(intersections, intersection.comps);
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
      if (retIndex == -1 && intersections.at(i).t > 0)
      {
        retIndex = i;
      }
      else if (intersections.at(i).t > 0 &&
               intersections.at(i).t < intersections.at(retIndex).t)
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
void getRefractiveIndexFromTo(std::vector<Intersection<T>> &intersections, std::unique_ptr<IntersectionParameters> &comps)
{
  std::vector<T *> objects;
  Intersection<T> *hitIntersection = hit(intersections);

  for (auto &intersection : intersections)
  {
    if (&intersection == hitIntersection)
    {
      if (objects.empty())
        comps->n1 = 1.f;
      else
        comps->n1 = objects.back()->material->refractiveIndex;
    }

    typename std::vector<T *>::iterator position = std::find(objects.begin(), objects.end(), intersection.shapePtr.get());
    if (position != objects.end())
      objects.erase(position);
    else
      objects.push_back(intersection.shapePtr.get());

    if (&intersection == hitIntersection)
    {
      if (objects.empty())
        comps->n2 = 1.f;
      else
        comps->n2 = objects.back()->material->refractiveIndex;
      break;
    }
  }
}

} // namespace Geometry