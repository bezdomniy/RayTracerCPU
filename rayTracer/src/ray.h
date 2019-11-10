#pragma once

#include <glm/glm.hpp>
#include <ostream>

class Ray
{
public:
	Ray();
	Ray(glm::vec4, glm::vec4);
	~Ray();

	glm::vec4 position(float);

    glm::vec4 origin;
    glm::vec4 direction;

	Ray transform(glm::mat4& m);

private:

};

std::ostream& operator<< (std::ostream &out, Ray const& r);
