#include "environment.h"

Environment::Environment(glm::vec4 g, glm::vec4 w)
{
    gravity = g;
    wind = w;
}

Environment::~Environment()
{
}
