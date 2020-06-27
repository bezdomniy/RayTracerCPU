#pragma once

#include <vector>
#include <string>
#include <stdio.h>
#include <glm/glm.hpp>
#include <fstream>
#include <sstream>
#include <memory>

#include "group.h"

class Model
{
public:
	Model();
	Model(std::string const &path);
	~Model();

	// std::vector<glm::dvec3> vertices;
	// std::vector<glm::dvec2> uvs;
	// std::vector<glm::dvec3> normals;

	std::shared_ptr<Group> mesh;
};
