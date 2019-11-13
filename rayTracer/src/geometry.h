#pragma once

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/intersect.hpp>
#include <glm/glm.hpp>
#include <vector>
// #include <utility>
// #include <optional>
#include <memory>
#include <cmath>
#include <glm/gtc/matrix_inverse.hpp>
#include "ray.h"
#include "sphere.h"

namespace Geometry {
    struct Intersection {
        float t;
        std::shared_ptr<Sphere> spherePtr;
    };

    std::vector<Intersection> raySphereIntersection(Ray& r, Sphere& s);

    Intersection* hit(std::vector<Intersection>& intersections);
    std::vector<Intersection> intersectRaySphere(Ray& ray, Sphere& sphere);
}