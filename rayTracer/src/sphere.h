#pragma once

#include <glm/glm.hpp>

class Sphere
{
public:
	Sphere(unsigned int, glm::vec3, float);
	~Sphere();

    unsigned int id;
	glm::vec3 position;
	float radius;
private:

};

