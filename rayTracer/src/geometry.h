#pragma once

#define GLM_ENABLE_EXPERIMENTAL
#define _CRT_SECURE_NO_WARNINGS

#include <glm/gtx/intersect.hpp>
#include <glm/glm.hpp>
#include <vector>
// #include <utility>
// #include <optional>
#include <memory>
#include <cmath>
#include <algorithm>
#include <glm/gtc/matrix_inverse.hpp>
#include "ray.h"
#include "sphere.h"
#include "pointLight.h"
// #include "world.h"

namespace Geometry {
    struct IntersectionParameters {
        glm::vec4 point;
        glm::vec4 normalv;
        glm::vec4 eyev;
        bool inside;
    };

    struct Intersection {
        float t;
        std::shared_ptr<Sphere> spherePtr;
        std::unique_ptr<IntersectionParameters> comps;
    };

    void getIntersectionParameters(Intersection& intersection, Ray& ray);

    bool compareIntersection(Intersection& i1, Intersection& i2);

    // std::vector<Intersection> raySphereIntersection(Ray& r, Sphere& s);

    Intersection* hit(std::vector<Intersection>& intersections);
    std::vector<Intersection> intersectRaySphere(Ray& ray, Sphere& sphere);
    // std::vector<Intersection> intersectRayWorld(Ray& ray, World& world);
    glm::vec3 lighting(std::shared_ptr<Material> material, std::shared_ptr<PointLight> light, glm::vec4 point, glm::vec4 eyev, glm::vec4 normalv, bool inShadow);
    // glm::vec3 shadeHit(Intersection* hit, World& world);
}