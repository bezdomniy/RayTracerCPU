#include "shape.h"

Shape::Shape(unsigned int id, glm::vec4 position)
{
    this->id = id;
    this->position = position;
}

Shape::~Shape()
{
}