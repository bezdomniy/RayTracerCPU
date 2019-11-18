#include "world.h"

World::World() {}

World::~World() {}

void World::addLight(std::shared_ptr<PointLight> &light)
{
  this->lights.push_back(light);
}

void World::addSphere(Sphere &sphere)
{
  ;
  // std::make_shared<Author>(std::move(t_author));
  this->spheres.push_back(std::make_shared<Sphere>(std::move(sphere)));
}

void World::addShape(Shape &shape)
{
  try
  {
    std::shared_ptr<Sphere> sphere_ptr =
        std::make_shared<Sphere>(std::move(*dynamic_cast<Sphere *>(&shape)));
    this->spheres.push_back(sphere_ptr);
  }
  catch (std::bad_cast &bc)
  {
    std::cerr << "bad_cast caught: " << bc.what() << '\n';
    // return;
  }
}

std::vector<Geometry::Intersection> World::intersectRay(Ray &ray)
{
  std::vector<Geometry::Intersection> ret;

  for (auto &sphere : this->spheres)
  {
    std::vector<Geometry::Intersection> next =
        sphere->intersectRay(ray);
    ret.insert(ret.end(), std::make_move_iterator(next.begin()),
               std::make_move_iterator(next.end()));
  }

  std::sort(ret.begin(), ret.end(), Geometry::compareIntersection);

  return ret;
}
