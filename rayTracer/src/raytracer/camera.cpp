#include "camera.h"

Camera::Camera(Vec4 position, Vec4 centre, Vec4 up, int hsize,
               int vsize, Float fov) : Shape()
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

void Camera::updateTransform()
{
  Mat4 transform = glm::lookAt(Vec3(position),
                               Vec3(this->centre), Vec3(this->up));
  this->inverseTransform = glm::affineInverse(transform);
}

Camera::~Camera() {}

void Camera::setPixelSize()
{
  Float halfView = glm::tan(this->fov / 2);
  Float aspect = (Float)this->hsize / (Float)this->vsize;

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
  this->pixelSize = (this->halfWidth * 2.0) / (Float)this->hsize;
}

Ray Camera::rayForPixel(Float px, Float py, int currentRayNumber, int sqrtRaysPerPixel, Float halfSubPixelSize)
{
  int subPixelRowNumber = currentRayNumber / sqrtRaysPerPixel;
  int subPixelColNumber = currentRayNumber % sqrtRaysPerPixel;
  Float subPixelxOffset = halfSubPixelSize * subPixelColNumber;
  Float subPixelyOffset = halfSubPixelSize * subPixelRowNumber;

  Float xOffset = (px + subPixelxOffset) * this->pixelSize;
  Float yOffset = (py + subPixelyOffset) * this->pixelSize;

  Float worldX = this->halfWidth - xOffset;
  Float worldY = this->halfHeight - yOffset;

  Vec4 pixel = this->inverseTransform *
               Vec4(worldX, worldY, -1.0, 1.0);
  Vec4 origin =
      this->inverseTransform * Vec4(0.0, 0.0, 0.0, 1.0);
  Vec4 direction = glm::normalize(pixel - origin);
  // Vec4 direction = pixel - origin;

  return Ray(origin, direction);
}

Vec4 Camera::normalAt(const Vec4 &point)
{
  return Vec4();
}

Vec4 Camera::normalAt(const Vec4 &point, const Vec2 &uv)
{
  return normalAt(point);
}

std::pair<Vec4, Vec4> Camera::bounds() const
{
  return std::pair<Vec4, Vec4>();
}

void Camera::intersectRay(Ray &ray, std::vector<Geometry::Intersection<Shape>> &intersections) { return; }

std::string Camera::type()
{
  return "Camera";
}
