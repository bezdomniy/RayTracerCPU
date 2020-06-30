#pragma once

#include <vector>
#include <string>
#include <stdio.h>
#include <glm/glm.hpp>
#include <fstream>
#include <sstream>
#include <memory>
#include <utility>

#include "group.h"

class Model
{
private:
	std::shared_ptr<Group> buildBoundingVolumeHierarchy(std::vector<std::shared_ptr<Shape>> &shapes);
	std::pair<glm::dvec4, glm::dvec4> mergeBounds(const std::pair<glm::dvec4, glm::dvec4> b1, const std::pair<glm::dvec4, glm::dvec4> b2);

public:
	Model();
	Model(std::string const &path, bool buildBVH = true);
	~Model();

	// std::vector<glm::dvec3> vertices;
	// std::vector<glm::dvec2> uvs;
	// std::vector<glm::dvec3> normals;

	std::shared_ptr<Group> mesh;
};
