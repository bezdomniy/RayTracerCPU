#pragma once

#include <fstream>
#include <glm/glm.hpp>
#include <string>
#include <vector>


class Canvas {
private:
  std::vector<glm::vec3> pixels;
  void _writeRgbString(float f, bool &newLine, int &charsInLine,
                       std::ofstream *streamPtr);

public:
  Canvas();
  Canvas(unsigned int, unsigned int);
  ~Canvas();

  unsigned int width;
  unsigned int height;

  void writePixel(unsigned int, unsigned int, glm::vec3);
  glm::vec3 getPixel(unsigned int, unsigned int);

  void clear(glm::vec3);

  void writeToPPM(const std::string &, bool invertY);
};
