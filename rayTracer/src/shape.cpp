#include "shape.h"

Shape::Shape()
{
  this->transform = glm::mat4(1.f);
  this->inverseTransform = glm::mat4(1.f);
}

Ray Shape::transformRay(Ray &ray)
{
  return ray.transform(this->inverseTransform);
}

void Shape::transformRayInPlace(Ray &ray)
{
  return ray.transformInPlace(this->inverseTransform);
}

void Shape::setMaterial(std::shared_ptr<Material> &mat)
{
  this->material = mat;
}

glm::vec3 Shape::patternAt(glm::vec4 point)
{
  // glm::mat4 shapeTransformInverse(glm::affineInverse(this->transform));
  glm::vec4 objectPoint = this->inverseTransform * point;

  glm::mat4 patternTransformInverse(glm::affineInverse(this->material->pattern->transform));
  glm::vec4 patternPoint = patternTransformInverse * objectPoint;

  return this->material->pattern->patternAt(patternPoint);
}

void Shape::multiplyTransform(glm::mat4 &transform)
{
  this->transform = transform * this->transform;
  // this->inverseTransform = glm::affineInverse(this->transform);
}

void Shape::calculateInverseTranform()
{
  this->inverseTransform = glm::affineInverse(this->transform);
}

Shape::~Shape() {}