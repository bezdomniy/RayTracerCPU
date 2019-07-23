#pragma once

#include <glm/glm.hpp>
#include <glm/gtx/rotate_vector.hpp>

class PathFinder
{
public:
	PathFinder();
	PathFinder(glm::vec3* worldPosition);
	~PathFinder();

	glm::vec3 findPathVector(float facingAngle);

private:
	glm::vec3* worldSpacePtr;
	//glm::vec3 forward;
};

