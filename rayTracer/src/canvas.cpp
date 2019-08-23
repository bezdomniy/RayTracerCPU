#include "canvas.h"

Canvas::Canvas(unsigned int w, unsigned int h)
{
    width = w;
    height = h;
    pixels.reserve(width * height);
}

Canvas::~Canvas()
{
}

void Canvas::writePixel(unsigned int x, unsigned int y, glm::vec3 colour) {
    pixels[y * width + x] = colour;
}

glm::vec3 Canvas::getPixel(unsigned int x, unsigned int y) {
    return pixels[y * width + x];
}

void Canvas::clear(glm::vec3 colour) {
    for (int i = 0; i < width * height; i++) {
        pixels[i] = colour;
    }
}

void Canvas::_writeRgbString(float f, bool& newLine, int& charsInLine, std::ofstream* streamPtr) {
    std::string c;
    if (f < 0.f) {
        c = "0";
    }
    else if (f > 1.f) {
        c = "255";
    }
    else {
        c = std::to_string(int(f * 255));
    }

    charsInLine += c.length() + 1;
    if (charsInLine >= 70) {
        *(streamPtr) << "\n";
        charsInLine = 0;
        newLine = true;
    }

    if (!newLine) *(streamPtr) << " ";
    *(streamPtr) << c;
    newLine = false;
} 

void Canvas::writeToPPM(const std::string& fileName, bool invertY = false) {
    std::ofstream out(fileName);

    if (out.fail()) {
        throw std::runtime_error("Failed to open file.");
        return;
    }

    out << "P3\n" << std::to_string(width) << " " << std::to_string(height) << "\n255";

    int charsInCurrentLine = 0;
    bool newLine = true;

    if (invertY) {
        for (int i = height - 1; i >= 0; i--) {
            out << "\n";
            charsInCurrentLine = 0;
            newLine = true;

            for (int j = 0; j < width; j++) {
                _writeRgbString(getPixel(j, i).x,  newLine, charsInCurrentLine, &out);
                _writeRgbString(getPixel(j, i).y,  newLine, charsInCurrentLine, &out);
                _writeRgbString(getPixel(j, i).z,  newLine, charsInCurrentLine, &out);
            }
        }
    }
    else {
        for (int i = 0; i < height; i++) {
        out << "\n";
        charsInCurrentLine = 0;
        newLine = true;

        for (int j = 0; j < width; j++) {
            _writeRgbString(getPixel(j, i).x,  newLine, charsInCurrentLine, &out);
            _writeRgbString(getPixel(j, i).y,  newLine, charsInCurrentLine, &out);
            _writeRgbString(getPixel(j, i).z,  newLine, charsInCurrentLine, &out);
        }
    }
    }


    out.close();
}
