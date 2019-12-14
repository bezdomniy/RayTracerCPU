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

public:
  Camera(unsigned int id, glm::vec4 position, glm::vec4 centre, glm::vec4 up, float hsize,
         float vsize, float fov);
  ~Camera();

  Ray rayForPixel(float px, float py);

  // glm::vec4 position;
  glm::vec4 centre;
  glm::vec4 up;

  float hsize;
  float vsize;
  float halfWidth;
  float halfHeight;
  float fov;
  float pixelSize;
  glm::mat4 transform;
};
