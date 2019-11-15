#pragma once

#include "canvas.h"
#include "camera.h"
// #include "shape.h"

#include "sphere.h"
#include "ray.h"
#include "geometry.h"
#include "pointLight.h"
#include "world.h"
#include <iostream>
#include <memory>
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>

class Renderer
{
private:
    std::shared_ptr<Camera> camera;
    
public:
    Renderer();
    Renderer(std::shared_ptr<Camera>& c);
    ~Renderer();

    glm::vec3 colourAt(Ray& ray, World& world);
    Canvas canvas;

    void render(World& world);
};
