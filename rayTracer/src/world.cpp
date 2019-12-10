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

void World::loadFromFile(std::string& fileName) {
    YAML::Node root = YAML::LoadFile(fileName);
}
