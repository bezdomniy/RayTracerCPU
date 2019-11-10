#pragma once

#include <glm/glm.hpp>
#include "shape.h"

class Sphere: public Shape
{
public:
	Sphere(unsigned int id, glm::vec4 position, float radius);
	~Sphere();

	float radius;
	glm::mat4 transform;
private:

};

