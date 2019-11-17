#pragma once

#include <glm/glm.hpp>
#include <vector>
#include <memory>
#include <iostream>
#include "shape.h"
#include "sphere.h"
#include "pointLight.h"
#include "ray.h"
#include "geometry.h"

class World
{
private:
    /* data */
public:
    World();
    ~World();

    void addShape(Shape& shape);
    void addSphere(Sphere& sphere);
    void addLight(std::shared_ptr<PointLight>& light);

    bool isShadowed(glm::vec4 point);
    std::vector<Geometry::Intersection> intersectRay(Ray& ray);
    glm::vec3 shadeHit(Geometry::Intersection* hit);

    std::vector<std::shared_ptr<Sphere>> spheres;
    std::vector<std::shared_ptr<PointLight>> lights;
};
