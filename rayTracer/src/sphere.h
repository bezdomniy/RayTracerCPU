#pragma once

#include <glm/glm.hpp>

class Sphere
{
public:
	Sphere(unsigned int, glm::vec4, float);
	~Sphere();

    unsigned int id;
	glm::vec4 position;
	float radius;
	glm::mat4 transform;
private:

};

