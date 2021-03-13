#include "shape.h"

Shape::Shape()
{
  this->transform = Mat4(1.0);
  this->inverseTransform = Mat4(1.0);
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

Vec3 Shape::patternAt(const Vec4 &point)
{
  // Mat4 shapeTransformInverse(glm::affineInverse(this->transform));
  Vec4 objectPoint = this->inverseTransform * point;

  Mat4 patternTransformInverse(glm::affineInverse(this->material->pattern->transform));
  Vec4 patternPoint = patternTransformInverse * objectPoint;

  return this->material->pattern->patternAt(patternPoint);
}

void Shape::multiplyTransform(Mat4 &transform)
{
  this->transform = transform * this->transform;
  // this->inverseTransform = glm::affineInverse(this->transform);
}

void Shape::calculateInverseTranform()
{
  this->inverseTransform = glm::affineInverse(this->transform);
}

Vec4 Shape::worldToObject(const Vec4 &point)
{
  if (this->parent)
  {
    // return Geometry::matVecMult(this->inverseTransform, this->parent->worldToObject(point));
    return this->inverseTransform * this->parent->worldToObject(point);
  }

  // return Geometry::matVecMult(this->inverseTransform, point);
  return this->inverseTransform * point;
}

Vec4 Shape::normalToWorld(const Vec4 &normal)
{
  Vec4 ret = glm::transpose(this->inverseTransform) * normal;
  ret.w = 0.0;
  ret = glm::normalize(ret);

  if (this->parent)
  {
    ret = this->parent->normalToWorld(ret);
  }

  return ret;
}

Vec4 Shape::boundsCentroid()
{
  return (Float).5 * this->bounds().first + (Float).5 * this->bounds().second;
}

Shape::~Shape()
{
}