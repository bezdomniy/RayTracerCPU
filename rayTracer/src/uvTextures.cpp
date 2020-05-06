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

glm::dvec3 CheckeredTexture::patternAt(glm::dvec2 uv)
{
  int u2 = (int)std::floor(uv.x * width);
  int v2 = (int)std::floor(uv.y * height);

  if ((u2 + v2) % 2 == 0)
  {
    return this->colourA;
  }
  else
  {
    return this->colourB;
  }
}

ImageTexture::ImageTexture(std::string const &path)
{
  this->texture = IMG_Load(path.c_str());

  // this->width = width;
  // this->height = height;
}

ImageTexture::~ImageTexture()
{
}

Uint32 ImageTexture::pixelFromSurface(int x, int y)
{
  int bpp = this->texture->format->BytesPerPixel;
  /* Here p is the address to the pixel we want to retrieve */
  Uint8 *p = (Uint8 *)this->texture->pixels + y * this->texture->pitch + x * bpp;

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

glm::dvec3 ImageTexture::rgbFromPixel(int x, int y)
{
  SDL_Color rgb;
  Uint32 pixel = pixelFromSurface(x, y);
  SDL_GetRGB(pixel, this->texture->format, &rgb.r, &rgb.g, &rgb.b);

  double r = (rgb.r / 255.);
  double g = (rgb.g / 255.);
  double b = (rgb.b / 255.);

  return glm::dvec3(r, g, b);
}

glm::dvec3 ImageTexture::patternAt(glm::dvec2 uv)
{
  double u = uv.x;
  double v = 1. - uv.y;

  double x = u * (this->texture->w - 1);
  double y = v * (this->texture->h - 1);

  return rgbFromPixel((int)std::round(x), (int)std::round(y));
}

// function uv_pattern_at(uv_image, u, v)
//   # flip v over so it matches the image layout, with y at the top
//   let v ← 1 - v

//   let x ← u * (uv_image.canvas.width - 1)
//   let y ← v * (uv_image.canvas.height - 1)

//   # be sure and round x and y to the nearest whole number
//   return pixel_at(uv_image.canvas, round(x), round(y))
// end function