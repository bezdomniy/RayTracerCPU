#pragma once

#include "types.h"
#include <ostream>

#include "geometry.h"

class Ray
{
public:
  Ray();
  Ray(Vec4, Vec4);
  ~Ray();

  Vec4 position(Float);

  Vec4 origin;
  Vec4 direction;

  Ray transform(Mat4 &m);
  Ray transform(Mat4 &m, Vec4 newOrigin);
  void transformInPlace(Mat4 &m);

private:
};

std::ostream &operator<<(std::ostream &out, Ray const &r);
