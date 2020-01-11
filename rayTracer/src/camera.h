#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_inverse.hpp>
#include <glm/gtc/matrix_transform.hpp>
// #include <glm/gtx/string_cast.hpp>
// #include <iostream>
#include "ray.h"
#include "shape.h"

class Camera : public Shape
{
private:
  void setPixelSize();
  virtual glm::vec4 normalAt(glm::vec4 point) override;
  virtual std::vector<Geometry::Intersection<Shape>> intersectRay(Ray &ray) override;
  virtual std::string type() override;

public:
  Camera(glm::vec4 position, glm::vec4 centre, glm::vec4 up, int hsize,
         int vsize, float fov);
  ~Camera();

  Ray rayForPixel(float px, float py, int currentRayNumber, int sqrtRaysPerPixel, float halfSubPixelSize);

  glm::vec4 position;
  glm::vec4 centre;
  glm::vec4 up;

  int hsize;
  int vsize;
  float halfWidth;
  float halfHeight;
  float fov;
  float pixelSize;
  glm::mat4 transform;
};
