// #include "geometry.h"

// using namespace Geometry;

// #include "glm/gtx/string_cast.hpp"
// #include <iostream>

// void Geometry::getIntersectionParameters(Intersection<T> &intersection, Ray &ray)
// {
//   intersection.comps = std::make_unique<IntersectionParameters>();
//   intersection.comps->point =
//       ray.origin + glm::normalize(ray.direction) * intersection.t;
//   intersection.comps->normalv =
//       intersection.shapePtr->normalAt(intersection.comps->point);
//   intersection.comps->eyev = -ray.direction;
//   intersection.comps->overPoint =
//       intersection.comps->point + intersection.comps->normalv * EPSILON;

//   if (glm::dot(intersection.comps->normalv, intersection.comps->eyev) < 0)
//   {
//     intersection.comps->inside = true;
//     intersection.comps->normalv = -intersection.comps->normalv;
//   }
//   else
//   {
//     intersection.comps->inside = false;
//   }
// }

// bool Geometry::compareIntersection(Intersection<> &i1, Intersection<> &i2)
// {
//   return (i1.t < i2.t);
// }

// Intersection<> *Geometry::hit(std::vector<Intersection<>> &intersections)
// {
//   int retIndex = -1;

//   if (!intersections.empty())
//   {
//     for (int i = 0; i < intersections.size(); i++)
//     {
//       // std::cout << intersections.at(i).t << " ";
//       if (retIndex == -1 && intersections.at(i).t > 0)
//       {
//         retIndex = i;
//       }
//       else if (intersections.at(i).t > 0 &&
//                intersections.at(i).t < intersections.at(retIndex).t)
//       {
//         retIndex = i;
//       }
//     }

//     if (retIndex != -1 && intersections.at(retIndex).t > 0)
//     {
//       return &intersections.at(retIndex);
//     }
//   }
//   return nullptr;
// }
