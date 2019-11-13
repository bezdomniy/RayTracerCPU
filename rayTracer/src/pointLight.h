#include "shape.h"

class PointLight: public Shape
{
private:
    /* data */
public:
    PointLight(unsigned int id, glm::vec4 position, glm::vec3 intensity);
    ~PointLight();

    glm::vec3 intensity;
};


