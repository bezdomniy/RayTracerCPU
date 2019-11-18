#pragma once

#include <glm/glm.hpp>

class Environment
{
private:

public:
    Environment(glm::vec4, glm::vec4);
    ~Environment();

    glm::vec4 gravity;
    glm::vec4 wind;
};

