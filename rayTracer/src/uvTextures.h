#pragma once
#include <glm/glm.hpp>

class UVTexture
{
private:
  /* data */
public:
  UVTexture(/* args */);
  ~UVTexture();

  virtual glm::dvec3 patternAt(glm::dvec2 uv) = 0;
};

class CheckeredTexture: public UVTexture
{
public:
  CheckeredTexture(glm::dvec3 colourA, glm::dvec3 colourB, int width, int height);
  ~CheckeredTexture();
  virtual glm::dvec3 patternAt(glm::dvec2 uv) override;

  glm::dvec3 colourA;
  glm::dvec3 colourB;
  int width;
  int height;
};


