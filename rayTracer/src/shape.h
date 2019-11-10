#pragma once

#include <glm/glm.hpp>

class Shape
{
private:
    /* data */
public:
    Shape(unsigned int id, glm::vec4 position);
    virtual ~Shape();

    unsigned int id;
	glm::vec4 position;
};


