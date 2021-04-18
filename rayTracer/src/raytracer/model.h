#pragma once

#include <vector>
#include <string>
#include <stdio.h>
#include <glm/glm.hpp>
#include <fstream>
#include <sstream>
#include <memory>
#include <utility>
#include <chrono>
#include <iostream>

#include "group.h"

class Model
{
private:
	static std::shared_ptr<Group> recursiveBuild(std::vector<std::shared_ptr<Shape>> &shapes, uint32_t start, uint32_t end);
	static std::pair<glm::dvec4, glm::dvec4> mergeBounds(const std::pair<glm::dvec4, glm::dvec4>& b1, const std::pair<glm::dvec4, glm::dvec4>& b2);
	static std::pair<glm::dvec4, glm::dvec4> mergeBounds(const std::pair<glm::dvec4, glm::dvec4>& b1, const glm::dvec4& p);

public:
	Model();
	Model(std::string const &path, bool buildBVH = true);
	Model(const Model &model);
	~Model();

	void build(std::string const &path, bool buildBVH);

	static std::shared_ptr<Group> buildBoundingVolumeHierarchy(std::vector<std::shared_ptr<Shape>> &shapes);

	// std::vector<glm::dvec3> vertices;
	// std::vector<glm::dvec2> uvs;
	// std::vector<glm::dvec3> normals;

	std::shared_ptr<Group> mesh;
};
