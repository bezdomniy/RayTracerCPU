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
  virtual glm::dvec4 normalAt(glm::dvec4 point) override;
  virtual glm::dvec4 normalAt(glm::dvec4 point, glm::dvec2 uv) override;
  virtual void intersectRay(Ray &ray, std::vector<Geometry::Intersection<Shape>> &intersections) override;
  virtual std::pair<glm::dvec4, glm::dvec4> bounds() override;
  virtual std::string type() override;

public:
  Camera(glm::dvec4 position, glm::dvec4 centre, glm::dvec4 up, int hsize,
         int vsize, double fov);
  ~Camera();

  Ray rayForPixel(double px, double py, int currentRayNumber, int sqrtRaysPerPixel, double halfSubPixelSize);
  void updateTransform();

  glm::dvec4 position;
  glm::dvec4 centre;
  glm::dvec4 up;

  int hsize;
  int vsize;
  double halfWidth;
  double halfHeight;
  double fov;
  double pixelSize;
  glm::dmat4 transform;
};
