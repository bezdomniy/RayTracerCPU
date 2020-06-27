#include "uvTextures.h"

UVTexture::UVTexture(/* args */)
{
}

UVTexture::~UVTexture()
{
}

CheckeredTexture::CheckeredTexture()
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

glm::dvec3 CheckeredTexture::patternAt(glm::dvec2 uv, int faceIndex)
{
  int u2 = (int)std::floor(uv.x * this->width);
  int v2 = (int)std::floor(uv.y * this->height);

  if ((u2 + v2) % 2 == 0)
  {
    return this->colourA;
  }
  else
  {
    return this->colourB;
  }
}

void CheckeredTexture::loadRight(std::string const &path)
{
}

void CheckeredTexture::loadLeft(std::string const &path)
{
}

void CheckeredTexture::loadUp(std::string const &path)
{
}

void CheckeredTexture::loadDown(std::string const &path)
{
}

void CheckeredTexture::loadFront(std::string const &path)
{
}

void CheckeredTexture::loadBack(std::string const &path)
{
}

ImageTexture::ImageTexture()
{
  this->textures.resize(6);
}

ImageTexture::ImageTexture(std::string const &path)
{
  this->textures.push_back(IMG_Load(path.c_str()));
}

ImageTexture::~ImageTexture()
{
}

Uint32 ImageTexture::pixelFromSurface(SDL_Surface *surface, int x, int y)
{
  int bpp = surface->format->BytesPerPixel;
  /* Here p is the address to the pixel we want to retrieve */
  Uint8 *p = (Uint8 *)surface->pixels + y * surface->pitch + x * bpp;

  switch (bpp)
  {
  case 1:
    return *p;
    break;

  case 2:
    return *(Uint16 *)p;
    break;

  case 3:
    if (SDL_BYTEORDER == SDL_BIG_ENDIAN)
      return p[0] << 16 | p[1] << 8 | p[2];
    else
      return p[0] | p[1] << 8 | p[2] << 16;
    break;

  case 4:
    return *(Uint32 *)p;
    break;

  default:
    return 0; /* shouldn't happen, but avoids warnings */
  }
}

glm::dvec3 ImageTexture::rgbFromPixel(SDL_Surface *surface, int x, int y)
{
  SDL_Color rgb;
  Uint32 pixel = pixelFromSurface(surface, x, y);
  SDL_GetRGB(pixel, surface->format, &rgb.r, &rgb.g, &rgb.b);

  double r = (rgb.r / 255.);
  double g = (rgb.g / 255.);
  double b = (rgb.b / 255.);

  return glm::dvec3(r, g, b);
}

glm::dvec3 ImageTexture::patternAt(glm::dvec2 uv, int faceIndex)
{
  double u = uv.x;
  double v = 1. - uv.y;

  double x = u * (this->textures.at(faceIndex)->w - 1);
  double y = v * (this->textures.at(faceIndex)->h - 1);

  return rgbFromPixel(this->textures.at(faceIndex), (int)std::round(x), (int)std::round(y));
}

void ImageTexture::loadRight(std::string const &path)
{
  this->textures.at(0) = IMG_Load(path.c_str());
}

void ImageTexture::loadLeft(std::string const &path)
{
  this->textures.at(1) = IMG_Load(path.c_str());
}

void ImageTexture::loadUp(std::string const &path)
{
  this->textures.at(2) = IMG_Load(path.c_str());
}

void ImageTexture::loadDown(std::string const &path)
{
  this->textures.at(3) = IMG_Load(path.c_str());
}

void ImageTexture::loadFront(std::string const &path)
{
  this->textures.at(4) = IMG_Load(path.c_str());
}

void ImageTexture::loadBack(std::string const &path)
{
  this->textures.at(5) = IMG_Load(path.c_str());
}