
#pragma once

#define GLM_ENABLE_EXPERIMENTAL
#define _CRT_SECURE_NO_WARNINGS

#include "types.h"
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

#include <glm/gtc/random.hpp>
#include <limits>
#include <memory>

// class Shape;

namespace Geometry
{
  // unsigned int unique_id()
  // {
  //   static unsigned int n = 0;
  //   return ++n;
  // }

  static const Float EPSILON = 0.000000001;
  // static const Float EPSILON = std::numeric_limits<Float>::epsilon();
  struct IntersectionParameters
  {
    Vec4 point;
    Vec4 normalv;
    Vec4 eyev;
    Vec4 reflectv;
    Vec4 overPoint;
    Vec4 underPoint;
    Float n1;
    Float n2;
    // bool inside; //TODO find way to remove
  };

  template <typename T>
  struct Intersection
  {
    Float t;
    T *shapePtr;
    Vec2 uv;
    //    unsigned int id;
    //    IntersectionParameters *comps;

    // Intersection(Float t, T *shapePtr, Vec2 uv)
    // {
    //   this->t = t;
    //   this->shapePtr = shapePtr;
    //   this->uv = uv;
    //   this->id = unique_id();
    // }

    // Intersection(Float t, T *shapePtr)
    // {
    //   this->t = t;
    //   this->shapePtr = shapePtr;
    //   this->id = unique_id();
    // }

    // Intersection(const Intersection &intersection)
    // {
    //   this->t = intersection.t;
    //   this->shapePtr = intersection.shapePtr;
    //   this->uv = intersection.uv;
    //   this->id = intersection.id;
    //   this->comps = std::move(intersection.comps);
    // }
  };

  template <typename T>
  void getRefractiveIndexFromTo(std::vector<Intersection<T>> &intersections,
                                Intersection<T> &hit, IntersectionParameters &comps)
  {
    std::vector<T *> objects;

    for (auto &intersection : intersections)
    {
      if (&intersection == &hit)
      {
        if (objects.empty())
          comps.n1 = 1.0;
        else
          comps.n1 = objects.back()->material->refractiveIndex;
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
          comps.n2 = 1.0;
        else
          comps.n2 = objects.back()->material->refractiveIndex;
        break;
      }
    }
  }

  template <typename T>
  IntersectionParameters getIntersectionParameters(Intersection<T> &intersection, Vec4 &rayOrigin, Vec4 &rayDirection,
                                                   std::vector<Intersection<T>> &intersections)
  {
    // intersection.comps = std::make_unique<IntersectionParameters>();
    IntersectionParameters comps = {};
    comps.point =
        rayOrigin + glm::normalize(rayDirection) * intersection.t;
    // TODO check that uv only null have using none-uv normalAt version
    comps.normalv =
        intersection.shapePtr->normalAt(comps.point, intersection.uv);
    comps.eyev = -rayDirection;

    if (glm::dot(comps.normalv, comps.eyev) < 0)
    {
      // intersection.comps->inside = true;
      comps.normalv = -comps.normalv;
    }
    // else
    // {
    //   intersection.comps->inside = false;
    // }

    comps.reflectv =
        glm::reflect(rayDirection, comps.normalv);
    comps.overPoint =
        comps.point + comps.normalv * EPSILON;
    comps.underPoint =
        comps.point - comps.normalv * EPSILON;

    getRefractiveIndexFromTo<T>(intersections, intersection, comps);

    return comps;
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
      if (intersection.t > 0.001)
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
  Float schlick(IntersectionParameters &comps)
  {
    Float cos = glm::dot(comps.eyev, comps.normalv);
    if (comps.n1 > comps.n2)
    {
      Float n = comps.n1 / comps.n2;
      Float sin2T = std::pow(n, 2) * (1.0 - std::pow(cos, 2));
      if (sin2T > 1.0)
        return 1.0;

      Float cosT = std::sqrt(1.0 - sin2T);
      cos = cosT;
    }
    Float r0 = std::pow((comps.n1 - comps.n2) / (comps.n1 + comps.n2), 2);
    return r0 + (1.0 - r0) * std::pow(1.0 - cos, 5);
  }

  template <typename T>
  std::pair<Float, Float> checkAxis(Float origin, Float direction, Float lowerBound = -1.0, Float upperBound = 1.0)
  {
    Float tmin_numerator = lowerBound - origin;
    Float tmax_numerator = upperBound - origin;

    std::pair<Float, Float> ret;

    if (std::abs(direction) >= EPSILON)
    {
      ret.first = tmin_numerator / direction;
      ret.second = tmax_numerator / direction;
    }
    else
    {
      ret.first = tmin_numerator * std::numeric_limits<Float>::infinity();
      ret.second = tmax_numerator * std::numeric_limits<Float>::infinity();
    }
    if (ret.first > ret.second)
      std::swap(ret.first, ret.second);

    return ret;
  }

  // inline Float vecDot(const Vec4 &x, const Vec4 &y);
  // Vec4 matVecMult(const Mat4 &m, const Vec4 &vec);

} // namespace Geometry
