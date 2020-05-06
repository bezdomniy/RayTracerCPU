#pragma once

#ifdef _WIN32
#define SDL_MAIN_HANDLED
#endif

#include <glm/glm.hpp>
#include <SDL.h>
#include <SDL_image.h>
#include <string>
#include <cmath>

class UVTexture
{
private:
  /* data */
public:
  UVTexture(/* args */);
  ~UVTexture();

  virtual glm::dvec3 patternAt(glm::dvec2 uv) = 0;
};

class CheckeredTexture : public UVTexture
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

class ImageTexture : public UVTexture
{
private:
  Uint32 pixelFromSurface(int x, int y);
  glm::dvec3 rgbFromPixel(int x, int y);

public:
  ImageTexture(std::string const &path);
  ~ImageTexture();
  virtual glm::dvec3 patternAt(glm::dvec2 uv) override;

  SDL_Surface *texture;
  int width;
  int height;
};
