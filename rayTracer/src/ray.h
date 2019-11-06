#pragma once

#include <glm/glm.hpp>

class Ray
{
public:
	Ray(glm::vec3, glm::vec3);
	~Ray();

	glm::vec3 positionAtTime(float);

    glm::vec3 origin;
    glm::vec3 direction;
private:


};

