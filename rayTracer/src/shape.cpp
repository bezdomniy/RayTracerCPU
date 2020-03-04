#include "shape.h"

Shape::Shape()
{
  this->transform = glm::dmat4(1.0);
  this->inverseTransform = glm::dmat4(1.0);
}

Ray Shape::transformRay(Ray &ray) {
  return ray.transform(this->inverseTransform);
}

void Shape::transformRayInPlace(Ray &ray) {
  return ray.transformInPlace(this->inverseTransform);
}

void Shape::setMaterial(std::shared_ptr<Material> &mat) {
  this->material = mat;
  this->materialSet = true;
}

glm::dvec3 Shape::patternAt(glm::dvec4 point)
{
  // glm::dmat4 shapeTransformInverse(glm::affineInverse(this->transform));
  glm::dvec4 objectPoint = this->inverseTransform * point;

  glm::dmat4 patternTransformInverse(glm::affineInverse(this->material->pattern->transform));
  glm::dvec4 patternPoint = patternTransformInverse * objectPoint;

  return this->material->pattern->patternAt(patternPoint);
}

void Shape::multiplyTransform(glm::dmat4 &transform)
{
  this->transform = transform * this->transform;
  // this->inverseTransform = glm::affineInverse(this->transform);
}

void Shape::calculateInverseTranform() {
  this->inverseTransform = glm::affineInverse(this->transform);
}

glm::dvec4 Shape::worldToObject(glm::dvec4 point)
{
  if (this->parent)
  {
    point = this->parent->worldToObject(point);
  }

  return this->inverseTransform * point;
}

glm::dvec4 Shape::normalToWorld(glm::dvec4 normal)
{
  normal = glm::transpose(this->inverseTransform) * normal;
  normal.w = 0.0;
  normal = glm::normalize(normal);

  if (this->parent)
  {
    normal = this->parent->normalToWorld(normal);
  }

  return normal;
}

Shape::~Shape() {}