#pragma once

#include "shape.h"
// #include "triangle.h"
#include "smoothTriangle.h"
#include "geometry.h"
#include <glm/glm.hpp>

#include <fstream>
#include <sstream>

#include "cereal/types/vector.hpp"
#include "cereal/types/utility.hpp"

typedef std::pair<glm::dvec4, glm::dvec4> NodeTLAS;
typedef SmoothTriangle NodeBLAS;

class Model : public Shape, public std::enable_shared_from_this<Model>
{
private:
	friend class cereal::access;
	template <class Archive>
	void serialize(Archive &archive)
	{
		archive(cereal::virtual_base_class<Shape>(this), blas, tlas);
	}

	std::vector<NodeBLAS> parseObjFile(std::string const &path);
	void recursiveBuild(std::vector<NodeBLAS> &unsortedShapes, uint32_t level, uint32_t branch, uint32_t start, uint32_t end, uint32_t tlasHeight);

	bool intersectAABB(const Ray &ray, const NodeTLAS &aabb);

	struct Node
	{
		int level;
		int branch;
	};

public:
	Model();
	void build(std::string const &path);
	Model(const Model &model);
	~Model();

	virtual void intersectRay(Ray &ray, std::vector<Geometry::Intersection<Shape>> &intersections) override;
	virtual glm::dvec4 normalAt(const glm::dvec4 &point) override;
	virtual glm::dvec4 normalAt(const glm::dvec4 &point, const glm::dvec2 &uv) override;
	virtual std::pair<glm::dvec4, glm::dvec4> bounds() const final override;
	virtual std::string type() override;

protected:
	std::vector<NodeTLAS> tlas;
	std::vector<NodeBLAS> blas;
};

CEREAL_REGISTER_TYPE(Model);

// #pragma once

// #include <vector>
// #include <string>
// #include <stdio.h>
// #include <glm/glm.hpp>
// #include <fstream>
// #include <sstream>
// #include <memory>
// #include <utility>
// #include <chrono>
// #include <iostream>

// #include "group.h"

// class Model
// {
// private:
// 	static std::shared_ptr<Group> recursiveBuild(std::vector<std::shared_ptr<Shape>> &shapes, uint32_t start, uint32_t end);
// 	static std::pair<glm::dvec4, glm::dvec4> mergeBounds(const std::pair<glm::dvec4, glm::dvec4>& b1, const std::pair<glm::dvec4, glm::dvec4>& b2);
// 	static std::pair<glm::dvec4, glm::dvec4> mergeBounds(const std::pair<glm::dvec4, glm::dvec4>& b1, const glm::dvec4& p);

// public:
// 	Model();
// 	Model(std::string const &path, bool buildBVH = true);
// 	Model(const Model &model);
// 	~Model();

// 	void build(std::string const &path, bool buildBVH);

// 	static std::shared_ptr<Group> buildBoundingVolumeHierarchy(std::vector<std::shared_ptr<Shape>> &shapes);

// 	// std::vector<glm::dvec3> vertices;
// 	// std::vector<glm::dvec2> uvs;
// 	// std::vector<glm::dvec3> normals;

// 	std::shared_ptr<Group> mesh;
// };
