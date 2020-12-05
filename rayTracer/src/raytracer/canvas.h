#pragma once

#include <fstream>
#include <glm/glm.hpp>
#include <string>
#include <vector>

class Canvas
{
private:
  void _writeRgbString(double f, bool &newLine, int &charsInLine,
                       std::ofstream *streamPtr);
  uint8_t rbgdoubleToInt(double f);
  char rbgdoubleToChar(double f);

public:
  Canvas();
  Canvas(unsigned int, unsigned int);
  ~Canvas();

  std::vector<glm::dvec3> pixels;

  unsigned int width;
  unsigned int height;

  void writePixel(unsigned int, unsigned int, glm::dvec3);
  glm::dvec3 getPixel(unsigned int, unsigned int);
  glm::ivec3 getPixelInt(unsigned int, unsigned int);

  void clear(glm::dvec3);

  void writeToPPM(const std::string &, bool invertY);
  std::pair<std::vector<char>, size_t> writeToRGBA(bool invertY);
};