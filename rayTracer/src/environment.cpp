#include "environment.h"

Environment::Environment(glm::vec3 g, glm::vec3 w)
{
    gravity = g;
    wind = w;
}

Environment::~Environment()
{
}
