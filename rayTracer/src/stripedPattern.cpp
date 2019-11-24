#include "stripedPattern.h"

StripedPattern::StripedPattern(glm::vec3 colourA, glm::vec3 colourB): Pattern()
{
    this->colourA = colourA;
    this->colourB = colourB;
}

StripedPattern::~StripedPattern()
{
}

glm::vec3 StripedPattern::patternAt(glm::vec4 point) {
    if ((int)(point.x * 3) % 2 == 0) return this->colourA;
    return this->colourB;
}