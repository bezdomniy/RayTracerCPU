#include "shape.h"

Shape::Shape()
{
  // this->transform = glm::dmat4(1.0);
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

std::shared_ptr<Material> &Shape::getMaterial()
{
  if (parent && !material)
    return parent->getMaterial();
  return material;
}

Vec3 Shape::patternAt(const Vec4 &point)
{
  Vec4 objectPoint;
  if (this->type() == "Triangle")
    objectPoint = point;
  else
    objectPoint = this->inverseTransform * point;

  Vec4 patternPoint = getMaterial()->pattern->inverseTransform * objectPoint;

  return getMaterial()->pattern->patternAt(patternPoint);
}

void Shape::calculateInverseTranform(Mat4 &transform)
{
  // if (this->type() == "Triangle")
  //   throw std::runtime_error("should not use triangle transform");

  Mat4 currentTransform = glm::affineInverse(this->inverseTransform);
  this->inverseTransform = glm::affineInverse(transform * currentTransform);
}

void Shape::calculateInverseTranform(std::vector<Mat4> &transforms)
{
  // if (this->type() == "Triangle")
  //   throw std::runtime_error("should not use triangle transform");

  Mat4 currentTransform = glm::affineInverse(this->inverseTransform);

  for (auto &mat : transforms)
  {
    currentTransform = mat * currentTransform;
  }

  this->inverseTransform = glm::affineInverse(currentTransform);
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
  Vec4 ret = normal;

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

Vec4 Shape::boundsCentroid() const
{
  const std::pair<Vec4, Vec4> b = bounds();
  Float half = 0.5;
  return half * b.first + half * b.second;
}

Shape::~Shape()
{
}
