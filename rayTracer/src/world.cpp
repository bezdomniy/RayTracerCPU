#include "world.h"

World::World() {}

World::~World() {}

void World::addLight(std::shared_ptr<PointLight> &light)
{
  this->lights.push_back(light);
}

void World::addShape(std::shared_ptr<Shape> &shape_ptr)
{
  this->shapes.push_back(shape_ptr);
}

std::vector<Geometry::Intersection<Shape>> World::intersectRay(Ray &ray)
{
  std::vector<Geometry::Intersection<Shape>> ret;

  for (auto &shape : this->shapes)
  {
    std::vector<Geometry::Intersection<Shape>> next =
        shape->intersectRay(ray);
    ret.insert(ret.end(), std::make_move_iterator(next.begin()),
               std::make_move_iterator(next.end()));
  }

  std::sort(ret.begin(), ret.end(), Geometry::compareIntersection<Shape>);

  return ret;
}

std::shared_ptr<Camera> World::loadFromFile(const std::string &fileName)
{
  if (!objectLoader)
    objectLoader = std::make_unique<ObjectLoader>();

  std::pair<std::shared_ptr<Camera>, std::vector<std::shared_ptr<Shape>>> ret =
      objectLoader->loadYaml(fileName);

  this->shapes = ret.second;
  return ret.first;
}
