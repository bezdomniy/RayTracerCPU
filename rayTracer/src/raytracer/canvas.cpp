#include "canvas.h"

Canvas::Canvas() {}

Canvas::Canvas(unsigned int w, unsigned int h)
{
  width = w;
  height = h;
  pixels.resize(width * height);
}

Canvas::~Canvas()
{
}

void Canvas::writePixel(unsigned int x, unsigned int y, glm::dvec3 colour)
{
  // if (height >= width)
  pixels[y * width + x] = colour;
  // else
  //   pixels[x * height + y] = colour;
}

glm::dvec3 Canvas::getPixel(unsigned int x, unsigned int y)
{
  // if (height >= width)
  return pixels[y * width + x];
  // return pixels[x * height + y];
}

glm::ivec3 Canvas::getPixelInt(unsigned int x, unsigned int y)
{
  glm::dvec3 pixel(getPixel(x, y));
  return glm::ivec3(rbgdoubleToInt(pixel.x), rbgdoubleToInt(pixel.y), rbgdoubleToInt(pixel.z));
}

void Canvas::clear(glm::dvec3 colour)
{
  for (int i = 0; i < width * height; i++)
  {
    pixels[i] = colour;
  }
}

uint8_t Canvas::rbgdoubleToInt(double f)
{
  uint8_t c;
  if (f < 0.0)
  {
    c = (uint8_t)0;
  }
  else if (f > 1.0)
  {
    c = (uint8_t)255;
  }
  else
  {
    c = (uint8_t)(f * 255);
  }

  return c;
}

void Canvas::_writeRgbString(double f, bool &newLine, int &charsInLine,
                             std::ofstream *streamPtr)
{
  std::string c = std::to_string(rbgdoubleToInt(f));

  charsInLine += c.length() + 1;
  if (charsInLine >= 70)
  {
    *(streamPtr) << "\n";
    charsInLine = 0;
    newLine = true;
  }

  if (!newLine)
    *(streamPtr) << " ";
  *(streamPtr) << c;
  newLine = false;
}

void Canvas::writeToPPM(const std::string &fileName, bool invertY = false)
{
  std::ofstream out(fileName);

  if (out.fail())
  {
    throw std::runtime_error("Failed to open file.");
    return;
  }

  out << "P3\n"
      << std::to_string(width) << " " << std::to_string(height) << "\n255";

  int charsInCurrentLine = 0;
  bool newLine = true;

  if (invertY)
  {
    for (int i = height - 1; i >= 0; i--)
    {
      out << "\n";
      charsInCurrentLine = 0;
      newLine = true;

      for (int j = 0; j < width; j++)
      {
        _writeRgbString(getPixel(j, i).x, newLine, charsInCurrentLine, &out);
        _writeRgbString(getPixel(j, i).y, newLine, charsInCurrentLine, &out);
        _writeRgbString(getPixel(j, i).z, newLine, charsInCurrentLine, &out);
      }
    }
  }
  else
  {
    for (int i = 0; i < height; i++)
    {
      out << "\n";
      charsInCurrentLine = 0;
      newLine = true;

      for (int j = 0; j < width; j++)
      {
        _writeRgbString(getPixel(j, i).x, newLine, charsInCurrentLine, &out);
        _writeRgbString(getPixel(j, i).y, newLine, charsInCurrentLine, &out);
        _writeRgbString(getPixel(j, i).z, newLine, charsInCurrentLine, &out);
      }
    }
  }
  out << "\n";
  out.close();
}

std::pair<std::vector<char>, size_t> Canvas::writeToRGBA(bool invertY)
{
  std::pair<std::vector<char>, size_t> ret;
  size_t size = (this->width * this->height * 4) * sizeof(char);

  std::vector<char> bytes(size, 0);
  unsigned int bytesIndex = 0;

  if (invertY)
  {
    for (int i = height - 1; i >= 0; i--)
    {
      for (int j = 0; j < width; j++)
      {
        bytes[bytesIndex] = rbgdoubleToInt(getPixel(j, i).x);
        bytes[bytesIndex + 1] = rbgdoubleToInt(getPixel(j, i).y);
        bytes[bytesIndex + 2] = rbgdoubleToInt(getPixel(j, i).z);
        bytes[bytesIndex + 3] = 127;

        bytesIndex += 4;
      }
    }
  }
  else
  {
    for (int i = 0; i < height; i++)
    {
      for (int j = 0; j < width; j++)
      {
        bytes[bytesIndex] = rbgdoubleToInt(getPixel(j, i).x);
        bytes[bytesIndex + 1] = rbgdoubleToInt(getPixel(j, i).y);
        bytes[bytesIndex + 2] = rbgdoubleToInt(getPixel(j, i).z);
        bytes[bytesIndex + 3] = 127;

        bytesIndex += 4;
      }
    }
  }

  ret.first = bytes;
  ret.second = size;

  return ret;
}