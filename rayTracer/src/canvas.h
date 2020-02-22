#pragma once

#include <fstream>
#include <glm/glm.hpp>
#include <string>
#include <vector>

class Canvas {
private:
  std::vector<glm::dvec3> pixels;
  void _writeRgbString(double f, bool &newLine, int &charsInLine,
                       std::ofstream *streamPtr);
  uint8_t rbgdoubleToInt(double f);

public:
  Canvas();
  Canvas(unsigned int, unsigned int);
  ~Canvas();

  unsigned int width;
  unsigned int height;

  void writePixel(unsigned int, unsigned int, glm::dvec3);
  glm::dvec3 getPixel(unsigned int, unsigned int);

  void clear(glm::dvec3);

  void writeToPPM(const std::string &, bool invertY);
  std::pair<std::vector<uint8_t>, size_t> writeToRGBA(bool invertY);
};
