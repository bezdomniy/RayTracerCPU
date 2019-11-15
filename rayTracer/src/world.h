#pragma once

#include <glm/glm.hpp>
#include <vector>
#include <memory>
#include <iostream>
#include "shape.h"
#include "sphere.h"
#include "pointLight.h"

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

    std::vector<std::shared_ptr<Sphere>> spheres;
    std::vector<std::shared_ptr<PointLight>> lights;
};


