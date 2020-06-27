#pragma once

#ifdef _WIN32
#define SDL_MAIN_HANDLED
#include <SDL.h>
#include <SDL_image.h>
#else
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#endif


#include <glm/glm.hpp>

#include <string>
#include <cmath>
#include <vector>

class UVTexture
{
public:
  UVTexture(/* args */);
  ~UVTexture();

  virtual void loadRight(std::string const &path) = 0;
  virtual void loadLeft(std::string const &path) = 0;
  virtual void loadUp(std::string const &path) = 0;
  virtual void loadDown(std::string const &path) = 0;
  virtual void loadFront(std::string const &path) = 0;
  virtual void loadBack(std::string const &path) = 0;

  virtual glm::dvec3 patternAt(glm::dvec2 uv, int faceIndex = 0) = 0;
};

class CheckeredTexture : public UVTexture
{
private:
  virtual void loadRight(std::string const &path) override;
  virtual void loadLeft(std::string const &path) override;
  virtual void loadUp(std::string const &path) override;
  virtual void loadDown(std::string const &path) override;
  virtual void loadFront(std::string const &path) override;
  virtual void loadBack(std::string const &path) override;

public:
  CheckeredTexture();
  CheckeredTexture(glm::dvec3 colourA, glm::dvec3 colourB, int width, int height);
  ~CheckeredTexture();
  virtual glm::dvec3 patternAt(glm::dvec2 uv, int faceIndex = 0) override;

  glm::dvec3 colourA;
  glm::dvec3 colourB;
  int width;
  int height;
};

class ImageTexture : public UVTexture
{
private:
  Uint32 pixelFromSurface(SDL_Surface *surface, int x, int y);
  glm::dvec3 rgbFromPixel(SDL_Surface *surface, int x, int y);

public:
  ImageTexture();
  ImageTexture(std::string const &path);
  ~ImageTexture();

  virtual void loadRight(std::string const &path) override;
  virtual void loadLeft(std::string const &path) override;
  virtual void loadUp(std::string const &path) override;
  virtual void loadDown(std::string const &path) override;
  virtual void loadFront(std::string const &path) override;
  virtual void loadBack(std::string const &path) override;

  virtual glm::dvec3 patternAt(glm::dvec2 uv, int faceIndex = 0) override;

  std::vector<SDL_Surface *> textures;
  int width;
  int height;
};
