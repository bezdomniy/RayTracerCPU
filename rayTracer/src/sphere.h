#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_inverse.hpp>
#include <memory>
#include "shape.h"
#include "material.h"

class Sphere: public Shape
{
public:
	Sphere(unsigned int id, glm::vec4 position, float radius);
	~Sphere();

	float radius;
	glm::mat4 transform;
	std::shared_ptr<Material> material;

	glm::vec4 normalAt(glm::vec4 point);
	void setMaterial(std::shared_ptr<Material> mat);
private:

};

