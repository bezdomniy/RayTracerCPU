#include "model.h"

NodeBLAS emptyNode{glm::dvec4(-1.0), glm::dvec4(-1.0), glm::dvec4(-1.0), glm::dvec4(-1.0), glm::dvec4(-1.0), glm::dvec4(-1.0)};
NodeTLAS emptyBounds{glm::dvec4(std::numeric_limits<double>::min()), glm::dvec4(std::numeric_limits<double>::min())};

void mergeBounds(NodeTLAS &self, const NodeTLAS &other)
{
	self.first = glm::dvec4(std::min(self.first.x, other.first.x),
							std::min(self.first.y, other.first.y),
							std::min(self.first.z, other.first.z), 1.);
	self.second = glm::dvec4(std::max(self.second.x, other.second.x),
							 std::max(self.second.y, other.second.y),
							 std::max(self.second.z, other.second.z), 1.);
}

void mergeBounds(NodeTLAS &self, const glm::dvec4 &p)
{
	self.first = glm::dvec4(std::min(self.first.x, p.x),
							std::min(self.first.y, p.y),
							std::min(self.first.z, p.z), 1.);
	self.second = glm::dvec4(std::max(self.second.x, p.x),
							 std::max(self.second.y, p.y),
							 std::max(self.second.z, p.z), 1.);
}

// TODO: refactor this to be a Shape with corresponding interface,
//       and implement an array based bvh
Model::Model() : Shape()
{
}

// #include <iostream>
// TODO use group information in model loading
// TODO use uvs and normals in model loading
void Model::build(std::string const &path)
{
	std::vector<NodeBLAS> unsortedShapes = parseObjFile(path);

	size_t sizeElements = Geometry::nextPowerOfTwo(unsortedShapes.size()) - 1;

	blas.reserve(sizeElements);
	tlas.resize(sizeElements);

	uint32_t tlasHeight = Geometry::log2int(unsortedShapes.size());

	auto start = std::chrono::high_resolution_clock::now();
	recursiveBuild(unsortedShapes, 0, 0, 0, unsortedShapes.size(), tlasHeight);

	auto stop = std::chrono::high_resolution_clock::now();

	std::cout << "Time to build BVH: " << std::chrono::duration_cast<std::chrono::milliseconds>(stop - start).count() << std::endl;
}

Model::Model(const Model &model)
{
	this->blas = model.blas;
	this->tlas = model.tlas;

	this->parent = model.parent;
	this->material = model.material;
	// this->transform = model.transform;
	this->inverseTransform = model.inverseTransform;
}

Model::~Model()
{
}

void Model::recursiveBuild(std::vector<NodeBLAS> &unsortedShapes, uint32_t level, uint32_t branch, uint32_t start, uint32_t end, uint32_t tlasHeight)
{
	NodeTLAS centroidBounds{
		glm::dvec4(std::numeric_limits<double>::infinity(), std::numeric_limits<double>::infinity(), std::numeric_limits<double>::infinity(), 1.0),
		glm::dvec4(-std::numeric_limits<double>::infinity(), -std::numeric_limits<double>::infinity(), -std::numeric_limits<double>::infinity(), 1.0)};

	for (auto it = unsortedShapes.begin() + start; it != unsortedShapes.begin() + end; ++it)
	{
		mergeBounds(centroidBounds, it->bounds());
	};

	glm::dvec4 diagonal = centroidBounds.second - centroidBounds.first;
	uint32_t splitDimension;

	if (diagonal.x > diagonal.y && diagonal.x > diagonal.z)
		splitDimension = 0;
	else if (diagonal.y > diagonal.z)
		splitDimension = 1;
	else
		splitDimension = 2;

	if (centroidBounds.first[splitDimension] == centroidBounds.second[splitDimension])
	{
		return;
	}
	else
	{
		int mid = (start + end) / 2;
		std::nth_element(&unsortedShapes[start], &unsortedShapes[mid],
						 &unsortedShapes[end - 1] + 1,
						 [splitDimension](const NodeBLAS &a, const NodeBLAS &b) {
							 return a.boundsCentroid()[splitDimension] < b.boundsCentroid()[splitDimension];
						 });

		uint32_t node = std::pow(2, level) + branch - 1;
		tlas.at(node) = centroidBounds;

		int nShapes = end - start;

		if (nShapes > 2)
		{
			recursiveBuild(unsortedShapes, level + 1, branch * 2, start, mid, tlasHeight);
			recursiveBuild(unsortedShapes, level + 1, (branch * 2) + 1, mid, end, tlasHeight); //TODO check branch calc
		}
		else
		{
			blas.push_back(unsortedShapes.at(start));
			blas.back().parent = shared_from_this();
			//
			if (nShapes == 2)
			{
				blas.push_back(unsortedShapes.at(start + 1));
				blas.back().parent = shared_from_this();
			}
			else
			{
				//          If there is only one leaf node, just add it twice to make all inner nodes have 2 children
				//                    blas.push_back(triangleParamsUnsorted.at(start));
				//                  Instead of adding it twice, add node with w value 0 and check in shader
				blas.push_back(emptyNode);

				//                  TODO: need to use sorted triangleParams array instead of this push approach, but also need to track emptynodes

				//                paddingIndices.push_back(((node-offset)*2)+1);
			}

			//            In case on unbalanced tree, add dummy branches
			if (level < tlasHeight)
			{
				level += 1;
				branch *= 2;

				uint32_t dummyNode = std::pow(2, level) + branch - 1;

				tlas.at(dummyNode) = centroidBounds;
				tlas.at(dummyNode + 1) = emptyBounds;

				blas.push_back(emptyNode);
				blas.push_back(emptyNode);
			}

			return;
		}
	}

	return;
}

// void Model::recursiveBuild(std::vector<NodeBLAS> &unsortedShapes, uint32_t level, uint32_t branch, uint32_t start, uint32_t end, uint32_t tlasHeight)
// {
// 	int nShapes = end - start;

// 	if (nShapes == 1)
// 	{
// 		// for (int i = start; i < end; ++i)
// 		// 	node->addChild(shapes.at(i));

// 		// node->addChild(shapes.at(start));

// 		//        if (nShapes == 2)
// 		//            node->addChild(shapes.at(start + 1));
// 		return;
// 	}

// 	NodeTLAS centroidBounds{
// 		glm::dvec4(std::numeric_limits<double>::infinity(), std::numeric_limits<double>::infinity(), std::numeric_limits<double>::infinity(), 1.0),
// 		glm::dvec4(-std::numeric_limits<double>::infinity(), -std::numeric_limits<double>::infinity(), -std::numeric_limits<double>::infinity(), 1.0)};

// 	for (auto it = unsortedShapes.begin() + start; it != unsortedShapes.begin() + end; ++it)
// 	{
// 		mergeBounds(centroidBounds, it->boundsCentroid());
// 	};

// 	glm::dvec4 diagonal = centroidBounds.second - centroidBounds.first;
// 	uint32_t splitDimension;

// 	if (diagonal.x > diagonal.y && diagonal.x > diagonal.z)
// 		splitDimension = 0;
// 	else if (diagonal.y > diagonal.z)
// 		splitDimension = 1;
// 	else
// 		splitDimension = 2;

// 	if (centroidBounds.first[splitDimension] == centroidBounds.second[splitDimension])
// 	{
// 		// TODO: investigate how this works with offsets
// 		for (int i = start; i < end; ++i)
// 			blas.push_back(unsortedShapes.at(i));
// 		return;
// 	}
// 	else
// 	{
// 		int mid; // = (start + end) / 2;

// 		// Allocate _BucketInfo_ for SAH partition buckets
// 		constexpr int nBuckets = 12;
// 		constexpr int maxPrimsInNode = 2; //Change to 2 to accomodate array based binary tree
// 		Geometry::BucketInfo buckets[nBuckets];

// 		// Initialize _BucketInfo_ for SAH partition buckets
// 		for (int i = start; i < end; ++i)
// 		{
// 			int b = nBuckets * Geometry::offset(unsortedShapes[i].boundsCentroid(), centroidBounds)[splitDimension];
// 			if (b == nBuckets)
// 				b = nBuckets - 1;

// 			buckets[b].count++;
// 			mergeBounds(buckets[b].bounds, unsortedShapes[i].bounds());
// 		}

// 		// Compute costs for splitting after each bucket
// 		double cost[nBuckets - 1];
// 		for (int i = 0; i < nBuckets - 1; ++i)
// 		{
// 			std::pair<glm::dvec4, glm::dvec4> b0, b1;
// 			int count0 = 0, count1 = 0;
// 			for (int j = 0; j <= i; ++j)
// 			{
// 				mergeBounds(b0, buckets[j].bounds);
// 				count0 += buckets[j].count;
// 			}
// 			for (int j = i + 1; j < nBuckets; ++j)
// 			{
// 				mergeBounds(b1, buckets[j].bounds);
// 				count1 += buckets[j].count;
// 			}

// 			std::pair<glm::dvec4, glm::dvec4> bounds{
// 				glm::vec4(std::numeric_limits<double>::infinity(), std::numeric_limits<double>::infinity(), std::numeric_limits<double>::infinity(), 1.f),
// 				glm::vec4(-std::numeric_limits<double>::infinity(), -std::numeric_limits<double>::infinity(), -std::numeric_limits<double>::infinity(), 1.f)};

// 			for (auto it = unsortedShapes.begin() + start; it != unsortedShapes.begin() + end; ++it)
// 				mergeBounds(bounds, it->bounds());

// 			cost[i] = 1 +
// 					  (count0 * Geometry::surfaceArea(b0) +
// 					   count1 * Geometry::surfaceArea(b1)) /
// 						  Geometry::surfaceArea(bounds);
// 		}

// 		// Find bucket to split at that minimizes SAH metric
// 		double minCost = cost[0];
// 		int minCostSplitBucket = 0;
// 		for (int i = 1; i < nBuckets - 1; ++i)
// 		{
// 			if (cost[i] < minCost)
// 			{
// 				minCost = cost[i];
// 				minCostSplitBucket = i;
// 			}
// 		}

// 		// Either create leaf or split primitives at selected SAH
// 		// bucket
// 		double leafCost = nShapes;
// 		if (nShapes > maxPrimsInNode || minCost < leafCost)
// 		{
// 			auto pmid = std::partition(
// 				&unsortedShapes[start], &unsortedShapes[end - 1] + 1,
// 				[=](const NodeBLAS &pi) {
// 					int b = nBuckets *
// 							Geometry::offset(pi.boundsCentroid(), centroidBounds)[splitDimension];
// 					if (b == nBuckets)
// 						b = nBuckets - 1;
// 					return b <= minCostSplitBucket;
// 				});
// 			mid = pmid - &unsortedShapes[0];
// 		}
// 		else
// 		{
// 			// Create leaf
// 			for (int i = start; i < end; ++i)
// 			{
// 				blas.push_back(unsortedShapes.at(i));
// 				// node->addChild(shapes.at(i));
// 			}

// 			return;
// 		}

// 		if (nShapes > 2)
// 		{
// 			uint32_t node = std::pow(2, level) + branch - 1;
// 			tlas.at(node) = centroidBounds;

// 			recursiveBuild(unsortedShapes, level + 1, branch * 2, start, mid, tlasHeight);
// 			recursiveBuild(unsortedShapes, level + 1, (branch * 2) + 1, mid, end, tlasHeight);
// 		}
// 		else
// 		{
// 			std::nth_element(&unsortedShapes[start], &unsortedShapes[mid],
// 							 &unsortedShapes[end - 1] + 1,
// 							 [splitDimension](const NodeBLAS &a, const NodeBLAS &b) {
// 								 return a.boundsCentroid()[splitDimension] < b.boundsCentroid()[splitDimension];
// 							 });

// 			blas.push_back(unsortedShapes.at(start));
// 			blas.back().parent = shared_from_this();
// 			//
// 			if (nShapes == 2)
// 			{
// 				blas.push_back(unsortedShapes.at(start + 1));
// 				blas.back().parent = shared_from_this();
// 			}
// 			else
// 			{
// 				blas.push_back(emptyNode);
// 			}

// 			if (level < tlasHeight)
// 			{
// 				level += 1;
// 				branch *= 2;

// 				uint32_t dummyNode = std::pow(2, level) + branch - 1;

// 				tlas.at(dummyNode) = centroidBounds;
// 				tlas.at(dummyNode + 1) = emptyBounds;

// 				blas.push_back(emptyNode);
// 				blas.push_back(emptyNode);
// 			}

// 			return;
// 		}
// 	}

// 	return;
// }

std::vector<NodeBLAS> Model::parseObjFile(std::string const &path)
{
	std::string line;

	std::ifstream in(path, std::ios::in);

	if (!in)
	{
		std::cout << "Impossible to open the file: " << path << std::endl;
	}

	std::vector<unsigned int> vertexIndices;
	//    std::vector<unsigned int> uvIndices;
	std::vector<unsigned int> normalIndices;
	std::vector<glm::dvec3> temp_vertices;
	// std::vector<glm::dvec2> temp_uvs;
	std::vector<glm::dvec3> temp_normals;

	while (std::getline(in, line))
	{
		if (line.substr(0, 2) == "v ")
		{
			std::istringstream v(line.substr(2));
			glm::dvec3 vertex;

			double x, y, z;
			v >> x;
			v >> y;
			v >> z;

			vertex = glm::dvec3(x, y, z);
			temp_vertices.push_back(vertex);
		}
		else if (line.substr(0, 2) == "vn")
		{
			std::istringstream vn(line.substr(3));
			glm::dvec3 normal;

			double x, y, z;
			vn >> x;
			vn >> y;
			vn >> z;

			normal = glm::dvec3(x, y, z);
			temp_normals.push_back(normal);
		}
		else if (line.substr(0, 2) == "f ")
		{
			unsigned int vertexIndex[3], uvIndex[3], normalIndex[3];
			std::istringstream f(line.substr(2));

			if (line.find('/') != std::string::npos)
			{
				for (int i = 0; i < 3; i++)
				{
					std::string vStr, vtStr, vnStr;
					std::getline(f, vStr, '/');
					std::getline(f, vtStr, '/');
					std::getline(f, vnStr, ' ');

					vertexIndex[i] = atoi(vStr.c_str());
					// uvIndex[i] = atoi(vtStr.c_str());
					normalIndex[i] = atoi(vnStr.c_str());
				}
			}
			else
			{
				std::string vStr[3];
				std::getline(f, vStr[0], ' ');
				std::getline(f, vStr[1], ' ');
				std::getline(f, vStr[2], ' ');
				vertexIndex[0] = atoi(vStr[0].c_str());
				vertexIndex[1] = atoi(vStr[1].c_str());
				vertexIndex[2] = atoi(vStr[2].c_str());
			}

			vertexIndices.push_back(vertexIndex[0]);
			vertexIndices.push_back(vertexIndex[1]);
			vertexIndices.push_back(vertexIndex[2]);
			// uvIndices.push_back(uvIndex[0]);
			// uvIndices.push_back(uvIndex[1]);
			// uvIndices.push_back(uvIndex[2]);
			normalIndices.push_back(normalIndex[0]);
			normalIndices.push_back(normalIndex[1]);
			normalIndices.push_back(normalIndex[2]);
		}
		else if (line.substr(0, 2) == "vt")
		{
			// std::istringstream vt(line.substr(3));
			// glm::dvec2 uv;
			// int U, V;
			// vt >> U;
			// vt >> V;
			// uv = glm::dvec2(U, V);
			// temp_uvs.push_back(uv);
		}
	}

	std::vector<NodeBLAS> triangleParams;
	triangleParams.reserve((vertexIndices.size() / 3));

	for (unsigned int i = 0; i < vertexIndices.size(); i += 3)
	{
		NodeBLAS nextNode;

		if (temp_normals.empty())
		{
			glm::dvec3 e1 = temp_vertices[vertexIndices[i + 1] - 1] - temp_vertices[vertexIndices[i] - 1];
			glm::dvec3 e2 = temp_vertices[vertexIndices[i + 2] - 1] - temp_vertices[vertexIndices[i] - 1];
			glm::dvec4 normal = glm::dvec4(glm::normalize(glm::cross(e2, e1)), 0.0);

			nextNode = NodeBLAS(
				temp_vertices[vertexIndices[i] - 1],
				temp_vertices[vertexIndices[i + 1] - 1],
				temp_vertices[vertexIndices[i + 2] - 1],
				normal,
				normal,
				normal);
		}
		else
		{
			nextNode = NodeBLAS(
				temp_vertices[vertexIndices[i] - 1],
				temp_vertices[vertexIndices[i + 1] - 1],
				temp_vertices[vertexIndices[i + 2] - 1],
				temp_normals[normalIndices[i] - 1],
				temp_normals[normalIndices[i + 1] - 1],
				temp_normals[normalIndices[i + 2] - 1]);
		}

		triangleParams.push_back(nextNode);
	}

	return triangleParams;
}

void Model::intersectRay(Ray &ray, std::vector<Geometry::Intersection<Shape>> &intersections)
{
	Ray transformedRay;
	transformedRay = transformRay(ray);

	std::vector<Node> stack;
	stack.reserve(30);

	Node nextNode{0, 0};
	stack.push_back(nextNode);

	double resT = -1.0;

	while (!stack.empty())
	{
		nextNode = stack.back();
		stack.pop_back();
		// nodeIdx = int(pow(2, nextNode.level))  - 1 + nextNode.branch;

		int firstChildIdx = int(pow(2, nextNode.level + 1)) - 1 + (nextNode.branch * 2);

		if (firstChildIdx < tlas.size())
		{
			if (intersectAABB(transformedRay, tlas[firstChildIdx]))
			{
				Node newNode = {nextNode.level + 1, nextNode.branch * 2};
				stack.push_back(newNode);
			}

			if (intersectAABB(transformedRay, tlas[firstChildIdx + 1]))
			{
				Node newNode = {nextNode.level + 1, (nextNode.branch * 2) + 1};
				stack.push_back(newNode);
			}
		}
		else
		{
			// primitiveIndices[topPrimivitiveIndices] = nextNode.branch * 2;
			// topPrimivitiveIndices += 1;
			double t1 = -1.0;
			double t2 = -1.0;
			int primIdx = nextNode.branch * 2;

			blas[primIdx].intersectRay(transformedRay, intersections);

			if (primIdx + 1 < blas.size()) //&& blas[primIdx + 1].point1.w > 0.0)
			{
				blas[primIdx + 1].intersectRay(transformedRay, intersections);
			}

			// // TODO: closest hit currently done elsewhere, maybe refactor to do here, uncomment below
			// if ((t1 > 0.001) && (t1 < resT) && (t2 < 0 || (t1 < t2)))
			// {
			// 	id = primIdx;
			// 	resT = t1;
			// }
			// else if ((t2 > 0.001) && (t2 < resT))
			// {
			// 	id = primIdx;
			// 	resT = t2;
			// }
		}
	}
}

glm::dvec4 Model::normalAt(const glm::dvec4 &point)
{
	glm::dvec4 objectPoint = worldToObject(point);
	glm::dvec4 objectNormal;

	// double points[3] = {std::abs(objectPoint.x), std::abs(objectPoint.y), std::abs(objectPoint.z)};
	// const int N = sizeof(points) / sizeof(double);
	// int indexMaxC = std::distance(points, std::max_element(points, points + N));

	// if (indexMaxC == 0)
	// 	objectNormal = glm::normalize(glm::dvec4(objectPoint.x, 0.0, 0.0, 0.0));
	// else if (indexMaxC == 1)
	// 	objectNormal = glm::normalize(glm::dvec4(0.0, objectPoint.y, 0.0, 0.0));
	// else
	// 	objectNormal = glm::normalize(glm::dvec4(0.0, 0.0, objectPoint.z, 0.0));

	return normalToWorld(objectNormal);
}

glm::dvec4 Model::normalAt(const glm::dvec4 &point, const glm::dvec2 &uv)
{
	return normalAt(point);
}

std::pair<glm::dvec4, glm::dvec4> Model::bounds() const
{
	return std::pair<glm::dvec4, glm::dvec4>(glm::dvec4(-1., -1., -1., 1.), glm::dvec4(1., 1., 1., 1.));
}

bool Model::intersectAABB(const Ray &ray, const NodeTLAS &aabb)
{
	double t_min = -std::numeric_limits<double>::infinity();
	double t_max = std::numeric_limits<double>::infinity();
	double temp, invD, t0, t1;

	for (int a = 0; a < 3; a++)
	{
		invD = 1.0 / ray.direction[a];
		t0 = (aabb.first[a] - ray.origin[a]) * invD;
		t1 = (aabb.second[a] - ray.origin[a]) * invD;
		if (invD < 0.0)
		{
			temp = t0;
			t0 = t1;
			t1 = temp;
		}
		// std::swap(t0, t1);
		t_min = t0 > t_min ? t0 : t_min;
		t_max = t1 < t_max ? t1 : t_max;
		if (t_max <= t_min)
		{
			return false;
		}
	}
	return true;
}

std::string Model::type() { return "Model"; }
