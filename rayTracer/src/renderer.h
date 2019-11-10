#pragma once

#include "canvas.h"
// #include "shape.h"

#include "sphere.h"
#include "ray.h"
#include "geometry.h"
#include <iostream>
#include <memory>
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>

class Renderer
{
private:
    std::shared_ptr<Canvas> canvas;
public:
    Renderer();
    Renderer(std::shared_ptr<Canvas> c);
    ~Renderer();

    void render(Shape& shape);
};


