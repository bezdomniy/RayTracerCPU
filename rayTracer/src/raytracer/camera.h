#pragma once

#include "types.h"
#include <glm/gtc/matrix_inverse.hpp>
#include <glm/gtc/matrix_transform.hpp>
// #include <glm/gtx/string_cast.hpp>
// #include <iostream>
#include "ray.h"
#include "shape.h"

class Camera : public Shape
{
private:
  friend class cereal::access;
  template <class Archive>
  void serialize(Archive &archive)
  {
    archive(cereal::virtual_base_class<Shape>(this), position, centre, up, hsize, vsize, halfWidth, halfHeight, fov, pixelSize);
  }

  // template <class Archive>
  // static void load_and_construct(Archive &archive, cereal::construct<Camera> &construct)
  // {
  //   Vec4 position;
  //   Vec4 centre;
  //   Vec4 up;

  //   int hsize;
  //   int vsize;
  //   Float fov;

  //   archive(position, centre, up, hsize, vsize, fov);
  //   construct(position, centre, up, hsize, vsize, fov);
  // }

  void setPixelSize();
  virtual Vec4 normalAt(const Vec4 &point) override;
  virtual Vec4 normalAt(const Vec4 &point, const Vec2 &uv) override;
  virtual void intersectRay(Ray &ray, std::vector<Geometry::Intersection<Shape>> &intersections) override;
  virtual std::pair<Vec4, Vec4> bounds() override;
  virtual std::string type() override;

public:
  Camera()
  {
  }
  Camera(Vec4 position, Vec4 centre, Vec4 up, int hsize,
         int vsize, Float fov);
  ~Camera();

  Ray rayForPixel(Float px, Float py, int currentRayNumber, int sqrtRaysPerPixel, Float halfSubPixelSize);
  void updateTransform();

  Vec4 position;
  Vec4 centre;
  Vec4 up;

  int hsize;
  int vsize;
  Float halfWidth;
  Float halfHeight;
  Float fov;
  Float pixelSize;
  // Mat4 transform;
};

CEREAL_REGISTER_TYPE(Camera);