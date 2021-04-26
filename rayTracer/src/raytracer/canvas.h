#pragma once

#include <fstream>
#include "types.h"
#include <string>
#include <vector>

class Canvas
{
private:
  void _writeRgbString(Float f, bool &newLine, int &charsInLine,
                       std::ofstream *streamPtr);
  uint8_t rbgdoubleToInt(Float f);
  char rbgdoubleToChar(Float f);

public:
  Canvas();
  Canvas(unsigned int, unsigned int);
  ~Canvas();

  std::vector<Vec3> pixels;

  unsigned int width;
  unsigned int height;

  void writePixel(unsigned int, unsigned int, Vec3);
  Vec3 getPixel(unsigned int, unsigned int);
  glm::ivec3 getPixelInt(unsigned int, unsigned int);

  void clear(Vec3);

  void writeToPPM(const std::string &, bool invertY);
  std::pair<std::vector<char>, size_t> writeToRGBA(bool invertY);
};
