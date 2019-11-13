#pragma once

#include "canvas.h"
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
    glm::vec3 lighting(std::shared_ptr<Material> material, std::shared_ptr<PointLight> light, glm::vec4 point, glm::vec4 eyev, glm::vec4 normalv);
    std::shared_ptr<Canvas> canvas;
    std::vector<std::shared_ptr<PointLight>> lights;
public:
    Renderer();
    Renderer(std::shared_ptr<Canvas> c);
    ~Renderer();

    void addLight(std::shared_ptr<PointLight>& light);
    void render(World& world);
    void render(Shape& shape);
};


