#include "camera.h"

Camera::Camera(glm::vec4 position, glm::vec4 centre, glm::vec4 up, int hsize,
               int vsize, float fov) : Shape()
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
  this->transform = glm::lookAt(glm::vec3(position),
                                glm::vec3(this->centre), glm::vec3(this->up));
}

Camera::~Camera() {}

void Camera::setPixelSize()
{
  float halfView = glm::tan(this->fov / 2);
  float aspect = (float)this->hsize / (float)this->vsize;

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
  this->pixelSize = (this->halfWidth * 2.f) / (float)this->hsize;
}

Ray Camera::rayForPixel(float px, float py, int currentRayNumber, int sqrtRaysPerPixel, float halfSubPixelSize)
{
  int subPixelRowNumber = currentRayNumber / sqrtRaysPerPixel;
  int subPixelColNumber = currentRayNumber % sqrtRaysPerPixel;
  float subPixelxOffset = halfSubPixelSize * subPixelColNumber;
  float subPixelyOffset = halfSubPixelSize * subPixelRowNumber;

  float xOffset = (px + subPixelxOffset) * this->pixelSize;
  float yOffset = (py + subPixelyOffset) * this->pixelSize;

  float worldX = this->halfWidth - xOffset;
  float worldY = this->halfHeight - yOffset;

  glm::vec4 pixel = glm::affineInverse(this->transform) *
                    glm::vec4(worldX, worldY, -1.f, 1.f);
  glm::vec4 origin =
      glm::affineInverse(this->transform) * glm::vec4(0.f, 0.f, 0.f, 1.f);
  glm::vec4 direction = glm::normalize(pixel - origin);

  return Ray(origin, direction);
}

glm::vec4 Camera::normalAt(glm::vec4 point)
{
  return glm::vec4();
}

void Camera::intersectRay(Ray& ray, std::vector<Geometry::Intersection<Shape>>& intersections) { return; }

std::string Camera::type()
{
  return "Camera";
}