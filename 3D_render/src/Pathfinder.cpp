#include "PathFinder.h"
#include "PathFinder.h"

PathFinder::PathFinder()
{
}

PathFinder::PathFinder(glm::vec3* worldPosition)
{
	worldSpacePtr = worldPosition;
}

PathFinder::~PathFinder()
{
}

glm::vec3 PathFinder::findPathVector(float facingAngle)
{
	glm::vec3 forward = glm::vec3(0.f, 0.f, 1.f);
	forward = glm::rotate(forward, glm::radians(facingAngle), glm::vec3(0.0f, -1.0f, 0.0f));

	return forward;
}
