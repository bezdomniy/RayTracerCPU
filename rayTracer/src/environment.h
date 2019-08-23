#pragma once

#include <glm/glm.hpp>

class Environment
{
private:

public:
    Environment(glm::vec3, glm::vec3);
    ~Environment();

    glm::vec3 gravity;
    glm::vec3 wind;
};

