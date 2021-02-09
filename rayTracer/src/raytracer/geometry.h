
#pragma once

#define GLM_ENABLE_EXPERIMENTAL
#define _CRT_SECURE_NO_WARNINGS

#include <glm/glm.hpp>
// #include <glm/gtx/intersect.hpp>

#include <glm/gtc/matrix_access.hpp>
#include <vector>
// #include "shape.h"
// #include "intersection.h"
// #include "pointLight.h"
// #include "ray.h"
// #include "material.h"
#include <algorithm>
#include <cmath>
#include <glm/gtc/matrix_inverse.hpp>
#include <limits>
#include <memory>

// class Shape;

namespace Geometry
{
  static const double EPSILON = 0.000000001;
  // static const double EPSILON = std::numeric_limits<double>::epsilon();
  struct IntersectionParameters
  {
    glm::dvec4 point;
    glm::dvec4 normalv;
    glm::dvec4 eyev;
    glm::dvec4 reflectv;
    glm::dvec4 overPoint;
    glm::dvec4 underPoint;
    double n1;
    double n2;
    bool inside; //TODO find way to remove
  };

  template <typename T>
  struct Intersection
  {
    double t;
    T *shapePtr;
    glm::dvec2 uv;
    std::unique_ptr<IntersectionParameters> comps;
  };

  template <typename T>
  void getRefractiveIndexFromTo(std::vector<Intersection<T>> &intersections,
                                Intersection<T> &hit)
  {
    std::vector<T *> objects;

    for (auto &intersection : intersections)
    {
      if (&intersection == &hit)
      {
        if (objects.empty())
          intersection.comps->n1 = 1.0;
        else
          intersection.comps->n1 = objects.back()->material->refractiveIndex;
      }

      typename std::vector<T *>::iterator position =
          std::find(objects.begin(), objects.end(), intersection.shapePtr);
      if (position != objects.end())
        objects.erase(position);
      else
        objects.push_back(intersection.shapePtr);

      if (&intersection == &hit)
      {
        if (objects.empty())
          intersection.comps->n2 = 1.0;
        else
          intersection.comps->n2 = objects.back()->material->refractiveIndex;
        break;
      }
    }
  }

  template <typename T>
  void getIntersectionParameters(Intersection<T> &intersection, glm::dvec4 &rayOrigin, glm::dvec4 &rayDirection,
                                 std::vector<Intersection<T>> &intersections)
  {
    intersection.comps = std::make_unique<IntersectionParameters>();
    intersection.comps->point =
        rayOrigin + glm::normalize(rayDirection) * intersection.t;
    // TODO check that uv only null have using none-uv normalAt version
    intersection.comps->normalv =
        intersection.shapePtr->normalAt(intersection.comps->point, intersection.uv);
    intersection.comps->eyev = -rayDirection;

    if (glm::dot(intersection.comps->normalv, intersection.comps->eyev) < 0)
    {
      intersection.comps->inside = true;
      intersection.comps->normalv = -intersection.comps->normalv;
    }
    else
    {
      intersection.comps->inside = false;
    }

    intersection.comps->reflectv =
        glm::reflect(rayDirection, intersection.comps->normalv);
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
    // int retIndex = -1;

    for (auto &intersection : intersections)
    {
      if (intersection.t > 0)
      {
        return &intersection;
      }
    }
    return nullptr;

    // if (!intersections.empty())
    // {
    //   for (int i = 0; i < intersections.size(); i++)
    //   {
    //     if ((retIndex == -1 && intersections.at(i).t > 0) ||
    //         (intersections.at(i).t > 0 &&
    //          intersections.at(i).t < intersections.at(retIndex).t))
    //     {
    //       retIndex = i;
    //     }
    //   }

    //   if (retIndex != -1 && intersections.at(retIndex).t > 0)
    //   {
    //     return &intersections.at(retIndex);
    //   }
    // }
    // return nullptr;
  }

  template <typename T>
  double schlick(std::unique_ptr<IntersectionParameters> &comps)
  {
    double cos = glm::dot(comps->eyev, comps->normalv);
    if (comps->n1 > comps->n2)
    {
      double n = comps->n1 / comps->n2;
      double sin2T = std::pow(n, 2) * (1.0 - std::pow(cos, 2));
      if (sin2T > 1.0)
        return 1.0;

      double cosT = std::sqrt(1.0 - sin2T);
      cos = cosT;
    }
    double r0 = std::pow((comps->n1 - comps->n2) / (comps->n1 + comps->n2), 2);
    return r0 + (1.0 - r0) * std::pow(1.0 - cos, 5);
  }

  template <typename T>
  std::pair<double, double> checkAxis(double origin, double direction, double lowerBound = -1.0, double upperBound = 1.0)
  {
    double tmin_numerator = lowerBound - origin;
    double tmax_numerator = upperBound - origin;

    std::pair<double, double> ret;

    if (std::abs(direction) >= EPSILON)
    {
      ret.first = tmin_numerator / direction;
      ret.second = tmax_numerator / direction;
    }
    else
    {
      ret.first = tmin_numerator * std::numeric_limits<double>::infinity();
      ret.second = tmax_numerator * std::numeric_limits<double>::infinity();
    }
    if (ret.first > ret.second)
      std::swap(ret.first, ret.second);

    return ret;
  }

  // inline double vecDot(const glm::dvec4 &x, const glm::dvec4 &y);
  // glm::dvec4 matVecMult(const glm::dmat4 &m, const glm::dvec4 &vec);

} // namespace Geometry