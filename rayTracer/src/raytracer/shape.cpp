#include "shape.h"

Shape::Shape()
{
  // this->transform = glm::dmat4(1.0);
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
  glm::dvec4 objectPoint;
  if (this->type() == "Triangle")
    objectPoint = point;
  else
    objectPoint = this->inverseTransform * point;

  glm::dvec4 patternPoint = getMaterial()->pattern->inverseTransform * objectPoint;

  return getMaterial()->pattern->patternAt(patternPoint);
}

// void Shape::multiplyTransform(glm::dmat4 &transform)
// {
//   // this->transform = transform * this->transform;
//   // this->inverseTransform = glm::affineInverse(this->transform);
// }

void Shape::calculateInverseTranform(glm::dmat4 &transform)
{
  // if (this->type() == "Triangle")
  //   throw std::runtime_error("should not use triangle transform");

  glm::dmat4 currentTransform = glm::affineInverse(this->inverseTransform);
  this->inverseTransform = glm::affineInverse(transform * currentTransform);
}

void Shape::calculateInverseTranform(std::vector<glm::dmat4> &transforms)
{
  // if (this->type() == "Triangle")
  //   throw std::runtime_error("should not use triangle transform");

  glm::dmat4 currentTransform = glm::affineInverse(this->inverseTransform);

  for (auto &mat : transforms)
  {
    currentTransform = mat * currentTransform;
  }

  this->inverseTransform = glm::affineInverse(currentTransform);
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
  glm::dvec4 ret = normal;

  if (this->type() != "Triangle")
  {
    ret = glm::transpose(this->inverseTransform) * normal;
    ret.w = 0.0;
    ret = glm::normalize(ret);
  }

  if (this->parent)
  {
    return this->parent->normalToWorld(ret);
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
