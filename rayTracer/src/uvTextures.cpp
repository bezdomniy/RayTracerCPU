#include "uvTextures.h"

UVTexture::UVTexture(/* args */)
{
}

UVTexture::~UVTexture()
{
}

CheckeredTexture::CheckeredTexture(glm::dvec3 colourA, glm::dvec3 colourB, int width, int height)
{
  this->colourA = colourA;
  this->colourB = colourB;
  this->width = width;
  this->height = height;
}

CheckeredTexture::~CheckeredTexture()
{
}

glm::dvec3 CheckeredTexture::patternAt(glm::dvec2 uv) {
  int u2 = (int)std::floor(uv.x * width);
  int v2 = (int)std::floor(uv.y * height);

  if ((u2 + v2) % 2 == 0) {
    return this->colourA;
  }
  else {
    return this->colourB;
  }
}
