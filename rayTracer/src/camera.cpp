#include "camera.h"

Camera::Camera(unsigned int id, glm::vec4 position, glm::vec4 centre, glm::vec4 up, float hsize,
               float vsize, float fov) : Shape(id, position)
{
  this->position = position;
  this->centre = centre;
  this->up = up;
  this->hsize = hsize;
  this->vsize = vsize;
  this->fov = fov;

  this->transform = glm::lookAt(glm::vec3(position),
                                glm::vec3(this->centre), glm::vec3(this->up));
  // std::cout << glm::to_string(transform) << std::endl;
  setPixelSize();
}

Camera::~Camera() {}

void Camera::setPixelSize()
{
  float halfView = glm::tan(this->fov / 2);
  float aspect = this->hsize / this->vsize;

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
  this->pixelSize = (this->halfWidth * 2) / this->hsize;
}

Ray Camera::rayForPixel(float px, float py)
{
  float xOffset = (px + 0.5f) * this->pixelSize;
  float yOffset = (py + 0.5f) * this->pixelSize;

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

std::vector<Geometry::Intersection<Shape>> Camera::intersectRay(Ray &ray)
{
  return std::vector<Geometry::Intersection<Shape>>();
}

std::string Camera::type()
{
  return "Camera";
}