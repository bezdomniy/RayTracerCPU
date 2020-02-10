#include "camera.h"

Camera::Camera(glm::dvec4 position, glm::dvec4 centre, glm::dvec4 up, int hsize,
               int vsize, double fov) : Shape()
{
  this->position = position;
  this->centre = centre;
  this->up = up;
  this->hsize = hsize;
  this->vsize = vsize;
  this->fov = fov;

  updateTransform();
  setPixelSize();
}

void Camera::updateTransform() {
  this->transform = glm::lookAt(glm::dvec3(position),
                                glm::dvec3(this->centre), glm::dvec3(this->up));
}

Camera::~Camera() {}

void Camera::setPixelSize()
{
  double halfView = glm::tan(this->fov / 2);
  double aspect = (double)this->hsize / (double)this->vsize;

  if (aspect >= 1)
  {
    this->halfWidth = halfView;
    this->halfHeight = halfView / aspect;
  }
  else
  {
    this->halfWidth = halfView * aspect;
    this->halfHeight = halfView;
  }
  this->pixelSize = (this->halfWidth * 2.0) / (double)this->hsize;
}

Ray Camera::rayForPixel(double px, double py, int currentRayNumber, int sqrtRaysPerPixel, double halfSubPixelSize)
{
  int subPixelRowNumber = currentRayNumber / sqrtRaysPerPixel;
  int subPixelColNumber = currentRayNumber % sqrtRaysPerPixel;
  double subPixelxOffset = halfSubPixelSize * subPixelColNumber;
  double subPixelyOffset = halfSubPixelSize * subPixelRowNumber;

  double xOffset = (px + subPixelxOffset) * this->pixelSize;
  double yOffset = (py + subPixelyOffset) * this->pixelSize;

  double worldX = this->halfWidth - xOffset;
  double worldY = this->halfHeight - yOffset;

  glm::dvec4 pixel = glm::affineInverse(this->transform) *
                    glm::dvec4(worldX, worldY, -1.0, 1.0);
  glm::dvec4 origin =
      glm::affineInverse(this->transform) * glm::dvec4(0.0, 0.0, 0.0, 1.0);
  glm::dvec4 direction = glm::normalize(pixel - origin);

  return Ray(origin, direction);
}

glm::dvec4 Camera::normalAt(glm::dvec4 point)
{
  return glm::dvec4();
}

void Camera::intersectRay(Ray& ray, std::vector<Geometry::Intersection<Shape>>& intersections) { return; }

std::string Camera::type()
{
  return "Camera";
}