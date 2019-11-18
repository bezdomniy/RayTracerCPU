#include "shape.h"

Shape::Shape(unsigned int id, glm::vec4 position)
{
  this->id = id;
  this->position = position;
  this->transform = glm::mat4(1.f);
}

std::vector<Geometry::Intersection<Shape>> Shape::intersectRay(Ray &ray)
{
  std::vector<Geometry::Intersection<Shape>> ret;
  return ret;
}

glm::vec4 Shape::normalAt(glm::vec4 point)
{
  glm::mat4 transformInverse(glm::affineInverse(this->transform));
  glm::vec4 objectPoint = transformInverse * point;
  glm::vec4 objectNormal = objectPoint - glm::vec4(0.f, 0.f, 0.f, 1.f);
  glm::vec4 worldNormal = glm::transpose(transformInverse) * objectNormal;
  worldNormal.w = 0.f;

  return glm::normalize(worldNormal);
}

Ray Shape::transformRay(Ray &ray)
{
  glm::mat4 inverseTransform = glm::affineInverse(this->transform);
  return ray.transform(inverseTransform);
}

void Shape::setMaterial(std::shared_ptr<Material> mat)
{
  this->material = mat;
}

Shape::~Shape() {}