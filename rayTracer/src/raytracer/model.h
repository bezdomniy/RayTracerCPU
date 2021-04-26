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

typedef std::pair<Vec4, Vec4> NodeTLAS;

typedef SmoothTriangle NodeBLAS;

// struct NodeBLAS
// {
// 	Vec4 p1;
// 	Vec4 p2;
// 	Vec4 p3;
// 	Vec4 n1;
// 	Vec4 n2;
// 	Vec4 n3;

// 	void intersectRay(Ray &ray, std::vector<Geometry::Intersection<Shape>> &intersections)
// 	{
// 		glm::dvec3 e1 = p2 - p1;
// 		glm::dvec3 e2 = p3 - p1;

// 		glm::dvec3 dirCrossE2 = glm::cross(glm::dvec3(ray.direction), e2);
// 		double det = glm::dot(e1, dirCrossE2);
// 		if (std::abs(det) < Geometry::EPSILON)
// 			return;

// 		double f = 1.0 / det;
// 		glm::dvec3 p1ToOrigin = ray.origin - p1;
// 		double u = f * glm::dot(p1ToOrigin, dirCrossE2);
// 		if (u < 0 || u > 1)
// 			return;

// 		glm::dvec3 originCrossE1 = glm::cross(p1ToOrigin, e1);
// 		double v = f * glm::dot(glm::dvec3(ray.direction), originCrossE1);
// 		if (v < 0 || (u + v) > 1)
// 			return;

// 		double t = f * glm::dot(e2, originCrossE1);

// 		intersections.push_back(Geometry::Intersection<Shape>{t, this, Vec2(u, v)});
// 	}
// };

class Model : public Shape,
			  public std::enable_shared_from_this<Model>
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
	virtual Vec4 normalAt(const Vec4 &point) override;
	virtual Vec4 normalAt(const Vec4 &point, const Vec2 &uv) override;
	virtual std::pair<Vec4, Vec4> bounds() const final override;
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
// 	static std::pair<Vec4, Vec4> mergeBounds(const std::pair<Vec4, Vec4>& b1, const std::pair<Vec4, Vec4>& b2);
// 	static std::pair<Vec4, Vec4> mergeBounds(const std::pair<Vec4, Vec4>& b1, const Vec4& p);

// public:
// 	Model();
// 	Model(std::string const &path, bool buildBVH = true);
// 	Model(const Model &model);
// 	~Model();

// 	void build(std::string const &path, bool buildBVH);

// 	static std::shared_ptr<Group> buildBoundingVolumeHierarchy(std::vector<std::shared_ptr<Shape>> &shapes);

// 	// std::vector<glm::dvec3> vertices;
// 	// std::vector<Vec2> uvs;
// 	// std::vector<glm::dvec3> normals;

// 	std::shared_ptr<Group> mesh;
// };
