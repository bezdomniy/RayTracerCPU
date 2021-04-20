#include "shape.h"

Shape::Shape()
{
  this->transform = glm::dmat4(1.0);
  this->inverseTransform = glm::dmat4(1.0);
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
  // this->materialSet = true;
}

std::shared_ptr<Material> &Shape::getMaterial()
{
  if (parent && !material)
    return parent->getMaterial();
  return material;
}

glm::dvec3 Shape::patternAt(const glm::dvec4 &point)
{
  // glm::dmat4 shapeTransformInverse(glm::affineInverse(this->transform));
  glm::dvec4 objectPoint = this->inverseTransform * point;

  glm::dmat4 patternTransformInverse(glm::affineInverse(getMaterial()->pattern->transform));
  glm::dvec4 patternPoint = patternTransformInverse * objectPoint;

  return getMaterial()->pattern->patternAt(patternPoint);
}

void Shape::multiplyTransform(glm::dmat4 &transform)
{
  this->transform = transform * this->transform;
  // this->inverseTransform = glm::affineInverse(this->transform);
}

void Shape::calculateInverseTranform()
{
  this->inverseTransform = glm::affineInverse(this->transform);
}

glm::dvec4 Shape::worldToObject(const glm::dvec4 &point)
{
  if (this->parent)
  {
    // return Geometry::matVecMult(this->inverseTransform, this->parent->worldToObject(point));
    return this->inverseTransform * this->parent->worldToObject(point);
  }

  // return Geometry::matVecMult(this->inverseTransform, point);
  return this->inverseTransform * point;
}

glm::dvec4 Shape::normalToWorld(const glm::dvec4 &normal)
{
  glm::dvec4 ret = glm::transpose(this->inverseTransform) * normal;
  ret.w = 0.0;
  ret = glm::normalize(ret);

  if (this->parent)
  {
    ret = this->parent->normalToWorld(ret);
  }

  return ret;
}

glm::dvec4 Shape::boundsCentroid() const
{
  const std::pair<glm::dvec4, glm::dvec4> b = bounds();
  return .5 * b.first + .5 * b.second;
}

Shape::~Shape()
{
}
