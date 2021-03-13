#pragma once

#include <vector>
#include <string>
#include <stdio.h>
#include "types.h"
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
	static std::pair<Vec4, Vec4> mergeBounds(const std::pair<Vec4, Vec4> b1, const std::pair<Vec4, Vec4> b2);
	static std::pair<Vec4, Vec4> mergeBounds(const std::pair<Vec4, Vec4> b1, const Vec4 p);

public:
	Model();
	Model(std::string const &path, bool buildBVH = true);
	Model(const Model &model);
	~Model();

	void build(std::string const &path, bool buildBVH);

	static std::shared_ptr<Group> buildBoundingVolumeHierarchy(std::vector<std::shared_ptr<Shape>> &shapes);

	// std::vector<Vec3> vertices;
	// std::vector<Vec2> uvs;
	// std::vector<Vec3> normals;

	std::shared_ptr<Group> mesh;
};
