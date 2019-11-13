#include "world.h"

World::World(/* args */)
{
}

World::~World()
{
}

void World::addLight(std::shared_ptr<PointLight>& light) {
    this->lights.push_back(light);
}

void World::addShape(Shape& shape) {
    std::shared_ptr<Sphere> sphere;
    try {
        sphere = std::shared_ptr<Sphere>(dynamic_cast<Sphere*>(&shape));
    }
    catch (std::bad_cast& bc) {
        std::cerr << "bad_cast caught: " << bc.what() << '\n';
        return;
    }

    this->spheres.push_back(sphere);
}