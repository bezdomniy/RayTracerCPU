#include "shape.h"

Shape::Shape(unsigned int id, glm::vec4 position)
{
  this->id = id;
  this->transform = glm::mat4(1.f);
}

Ray Shape::transformRay(Ray &ray)
{
  glm::mat4 inverseTransform = glm::affineInverse(this->transform);
  return ray.transform(inverseTransform);
}

void Shape::setMaterial(std::shared_ptr<Material> &mat)
{
  this->material = mat;
}

glm::vec3 Shape::patternAt(glm::vec4 point)
{
  glm::mat4 shapeTransformInverse(glm::affineInverse(this->transform));
  glm::vec4 objectPoint = shapeTransformInverse * point;

  glm::mat4 patternTransformInverse(glm::affineInverse(this->material->pattern->transform));
  glm::vec4 patternPoint = patternTransformInverse * objectPoint;

  return this->material->pattern->patternAt(patternPoint);
}

Shape::~Shape() {}