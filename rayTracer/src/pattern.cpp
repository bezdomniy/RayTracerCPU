#include "pattern.h"

Pattern::Pattern()
{
    this->transform = glm::mat4(1.f);
    this->inverseTransform = glm::mat4(1.f);
}

void Pattern::setTransform(glm::mat4 transform)
{
    this->transform = transform;
    // this->inverseTransform = glm::affineInverse(this->transform);
}

void Pattern::multiplyTransform(glm::mat4 &transform)
{
    this->transform = transform * this->transform;
    // this->inverseTransform = glm::affineInverse(this->transform);
}

void Pattern::calculateInverseTranform()
{
    this->inverseTransform = glm::affineInverse(this->transform);
}

Pattern::~Pattern()
{
}

ColourPattern::ColourPattern()
{
}

ColourPattern::ColourPattern(glm::vec3 colourA, glm::vec3 colourB) : Pattern()
{
    this->colourA = colourA;
    this->colourB = colourB;
}

void ColourPattern::setColour(glm::vec3 colour, int index)
{
    if (index == 0)
        this->colourA = colour;
    else if (index == 1)
        this->colourB = colour;
}

ColourPattern::~ColourPattern()
{
}