#include "world.h"

World::World() {}

World::~World()
{
  std::cout << "World destroyed." << std::endl;
}

void World::addLight(std::shared_ptr<PointLight> &light)
{
  this->lights.push_back(light);
}

void World::addShape(std::shared_ptr<Shape> &shape_ptr)
{
  this->shapes.push_back(shape_ptr);
}

void World::intersectRay(Ray &ray, std::vector<Geometry::Intersection<Shape>> &ret)
{
  ret.clear();
  // std::vector<Geometry::Intersection<Shape>> ret;
  // ret.reserve(this->shapes.size() * 2);

  // TODO - take out ray transform here and do on CUDA then have another for loop do the rest on cpu
  for (auto &shape : this->shapes)
  {
    shape->intersectRay(ray, ret);
  }

  std::sort(ret.begin(), ret.end(), Geometry::compareIntersection<Shape>);

  // return ret;
}

bool World::intersectRayShadow(Ray &ray, std::vector<Geometry::Intersection<Shape>> &intersectionBuffer, double distance)
{
  intersectionBuffer.clear();
  for (auto &shape : this->shapes)
  {
    if (shape->material->shadow)
    {
      shape->intersectRay(ray, intersectionBuffer);

      for (auto &intersection : intersectionBuffer)
      {
        if (intersection.t > 0.001 && intersection.t < distance)
          return true;
      }
    }
  }
  return false;
}
