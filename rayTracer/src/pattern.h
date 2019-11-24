#pragma once

#include <glm/glm.hpp>

class Pattern
{
private:
    /* data */
public:
    Pattern();
    virtual ~Pattern();

    virtual glm::vec3 patternAt(glm::vec4 point) = 0;

    glm::mat4 transform;
};

