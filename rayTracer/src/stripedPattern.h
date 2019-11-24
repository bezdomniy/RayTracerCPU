#pragma once

#include "pattern.h"

class StripedPattern: public Pattern
{
private:
    /* data */
public:
    StripedPattern(glm::vec3 colourA, glm::vec3 colourB);
    ~StripedPattern();
    virtual glm::vec3 patternAt(glm::vec4 point) override;

    glm::vec3 colourA;
    glm::vec3 colourB;
};


