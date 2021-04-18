#include "model.h"

// TODO: refactor this to be a Shape with corresponding interface,
//       and implement an array based bvh
Model::Model()
{
}

// #include <iostream>
// TODO use group information in model loading
// TODO use uvs and normals in model loading
Model::Model(std::string const &path, bool buildBVH)
{
	build(path, buildBVH);
}

Model::Model(const Model &model)
{
	this->mesh = std::make_shared<Group>(*(model.mesh));
}

void Model::build(std::string const &path, bool buildBVH)
{
	// this->mesh = std::make_shared<Group>();
	std::vector<unsigned int> vertexIndices, uvIndices, normalIndices;
	std::vector<glm::dvec3> temp_vertices;
	// std::vector<glm::dvec2> temp_uvs;
	std::vector<glm::dvec3> temp_normals;

	std::string line;

	std::ifstream in(path, std::ios::in);

	if (!in)
	{
		std::cout << "Impossible to open the file: " << path << std::endl;
		// return false;
	}

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

	std::vector<std::shared_ptr<Shape>> triangles;
	triangles.reserve(vertexIndices.size() / 3);

	for (unsigned int i = 0; i < vertexIndices.size(); i += 3)
	{
		std::shared_ptr<Shape> nextTriangle;

		if (temp_normals.empty())
		{
			nextTriangle = std::make_shared<Triangle>(temp_vertices[vertexIndices[i] - 1], temp_vertices[vertexIndices[i + 1] - 1], temp_vertices[vertexIndices[i + 2] - 1]);
		}
		else
		{
			nextTriangle = std::make_shared<SmoothTriangle>(temp_vertices[vertexIndices[i] - 1], temp_vertices[vertexIndices[i + 1] - 1], temp_vertices[vertexIndices[i + 2] - 1],
															temp_normals[normalIndices[i] - 1], temp_normals[normalIndices[i + 1] - 1], temp_normals[normalIndices[i + 2] - 1]);
		}

		triangles.push_back(nextTriangle);
	}

	if (buildBVH)
	{
		mesh = buildBoundingVolumeHierarchy(triangles);
	}
	else
	{
		mesh = std::make_shared<Group>();

		for (auto &triangle : triangles)
			mesh->addChild(triangle);
	}

	// for (unsigned int i = 0; i < vertexIndices.size(); i++)
	// {
	// 	unsigned int vertexIndex = vertexIndices[i];
	// 	glm::dvec3 vertex = temp_vertices[vertexIndex - 1];
	// 	this->vertices.push_back(vertex);
	// }
	// for (unsigned int i = 0; i < uvIndices.size(); i++)
	// {
	// 	unsigned int uvIndex = uvIndices[i];
	// 	glm::dvec2 uv = temp_uvs[uvIndex - 1];
	// 	this->uvs.push_back(uv);
	// }
	// for (unsigned int i = 0; i < normalIndices.size(); i++)
	// {
	// 	unsigned int normalIndex = normalIndices[i];
	// 	glm::dvec3 normal = temp_normals[normalIndex - 1];
	// 	this->normals.push_back(normal);
	// }
}

struct BucketInfo
{
	int count = 0;
	std::pair<glm::dvec4, glm::dvec4> bounds;
};

glm::dvec4 Offset(const glm::dvec4 &p, const std::pair<glm::dvec4, glm::dvec4> &bounds)
{
	glm::dvec4 o = p - bounds.first;
	if (bounds.second.x > bounds.first.x)
		o.x /= bounds.second.x - bounds.first.x;
	if (bounds.second.y > bounds.first.y)
		o.y /= bounds.second.y - bounds.first.y;
	if (bounds.second.z > bounds.first.z)
		o.z /= bounds.second.z - bounds.first.z;
	return o;
}

glm::dvec4 Diagonal(const std::pair<glm::dvec4, glm::dvec4> &bounds) { return bounds.second - bounds.first; }
double SurfaceArea(const std::pair<glm::dvec4, glm::dvec4> &bounds)
{
	glm::dvec4 d = Diagonal(bounds);
	return 2.0 * (d.x * d.y + d.x * d.z + d.y * d.z);
}

std::shared_ptr<Group> Model::recursiveBuild(std::vector<std::shared_ptr<Shape>> &shapes, uint32_t start, uint32_t end)
{
	std::shared_ptr<Group> node = std::make_shared<Group>();

	int nShapes = end - start;

	if (nShapes == 1)
	{
		// for (int i = start; i < end; ++i)
		// 	node->addChild(shapes.at(i));

		node->addChild(shapes.at(start));

		//        if (nShapes == 2)
		//            node->addChild(shapes.at(start + 1));
		return node;
	}
	else
	{
		// std::pair<glm::dvec4, glm::dvec4> centroidBounds;
		std::pair<glm::dvec4, glm::dvec4> centroidBounds{
			glm::vec4(std::numeric_limits<double>::infinity(), std::numeric_limits<double>::infinity(), std::numeric_limits<double>::infinity(), 1.f),
			glm::vec4(-std::numeric_limits<double>::infinity(), -std::numeric_limits<double>::infinity(), -std::numeric_limits<double>::infinity(), 1.f)};

		//for (const auto &shape : shapes)
		for (auto it = shapes.begin() + start; it != shapes.begin() + end; ++it)
			centroidBounds = mergeBounds(centroidBounds, (*it)->boundsCentroid());

		glm::dvec4 diagonal = centroidBounds.second - centroidBounds.first;
		int splitDimension;

		if (diagonal.x > diagonal.y && diagonal.x > diagonal.z)
			splitDimension = 0;
		else if (diagonal.y > diagonal.z)
			splitDimension = 1;
		else
			splitDimension = 2;

		int mid = (start + end) / 2;

		if (centroidBounds.first[splitDimension] == centroidBounds.second[splitDimension])
		{
			for (int i = start; i < end; ++i)
				node->addChild(shapes.at(i));
			return node;
		}
		else
		{
			if (false)
			{
				mid = (start + end) / 2;
				std::nth_element(&shapes[start], &shapes[mid],
								 &shapes[end - 1] + 1,
								 [splitDimension](const std::shared_ptr<Shape> &a, const std::shared_ptr<Shape> &b) {
									 return a->boundsCentroid()[splitDimension] < b->boundsCentroid()[splitDimension];
								 });
			}
			else
			{
				// Partition primitives using approximate SAH
				if (nShapes <= 2)
				{
					mid = (start + end) / 2;
					std::nth_element(&shapes[start], &shapes[mid],
									 &shapes[end - 1] + 1,
									 [splitDimension](const std::shared_ptr<Shape> &a, const std::shared_ptr<Shape> &b) {
										 return a->boundsCentroid()[splitDimension] < b->boundsCentroid()[splitDimension];
									 });
				}
				else
				{
					// Allocate _BucketInfo_ for SAH partition buckets
					constexpr int nBuckets = 12;
					constexpr int maxPrimsInNode = 4;
					BucketInfo buckets[nBuckets];

					// Initialize _BucketInfo_ for SAH partition buckets
					for (int i = start; i < end; ++i)
					{
						int b = nBuckets * Offset(shapes[i]->boundsCentroid(), centroidBounds)[splitDimension];
						if (b == nBuckets)
							b = nBuckets - 1;

						buckets[b].count++;
						buckets[b].bounds =
							mergeBounds(buckets[b].bounds, shapes[i]->bounds());
					}

					// Compute costs for splitting after each bucket
					double cost[nBuckets - 1];
					for (int i = 0; i < nBuckets - 1; ++i)
					{
						std::pair<glm::dvec4, glm::dvec4> b0, b1;
						int count0 = 0, count1 = 0;
						for (int j = 0; j <= i; ++j)
						{
							b0 = mergeBounds(b0, buckets[j].bounds);
							count0 += buckets[j].count;
						}
						for (int j = i + 1; j < nBuckets; ++j)
						{
							b1 = mergeBounds(b1, buckets[j].bounds);
							count1 += buckets[j].count;
						}

						std::pair<glm::dvec4, glm::dvec4> bounds{
							glm::vec4(std::numeric_limits<double>::infinity(), std::numeric_limits<double>::infinity(), std::numeric_limits<double>::infinity(), 1.f),
							glm::vec4(-std::numeric_limits<double>::infinity(), -std::numeric_limits<double>::infinity(), -std::numeric_limits<double>::infinity(), 1.f)};

						for (auto it = shapes.begin() + start; it != shapes.begin() + end; ++it)
							bounds = mergeBounds(bounds, (*it)->bounds());

						cost[i] = 1 +
								  (count0 * SurfaceArea(b0) +
								   count1 * SurfaceArea(b1)) /
									  SurfaceArea(bounds);
					}

					// Find bucket to split at that minimizes SAH metric
					double minCost = cost[0];
					int minCostSplitBucket = 0;
					for (int i = 1; i < nBuckets - 1; ++i)
					{
						if (cost[i] < minCost)
						{
							minCost = cost[i];
							minCostSplitBucket = i;
						}
					}

					// Either create leaf or split primitives at selected SAH
					// bucket
					float leafCost = nShapes;
					if (nShapes > maxPrimsInNode || minCost < leafCost)
					{
						auto pmid = std::partition(
							&shapes[start], &shapes[end - 1] + 1,
							[=](const std::shared_ptr<Shape> &pi) {
								int b = nBuckets *
										Offset(pi->boundsCentroid(), centroidBounds)[splitDimension];
								if (b == nBuckets)
									b = nBuckets - 1;
								return b <= minCostSplitBucket;
							});
						mid = pmid - &shapes[0];
					}
					else
					{
						// Create leaf
						for (int i = start; i < end; ++i)
						{
							node->addChild(shapes.at(i));
						}

						return node;
					}
				}
			}

			//            if (start !=mid)
			std::shared_ptr<Shape> leftChild = recursiveBuild(shapes, start, mid);

			//            if (end !=mid)
			std::shared_ptr<Shape> rightChild = recursiveBuild(shapes, mid, end);

			node->addChild(leftChild);
			node->addChild(rightChild);
		}
	}

	return node;
}

std::shared_ptr<Group> Model::buildBoundingVolumeHierarchy(std::vector<std::shared_ptr<Shape>> &shapes)
{
	auto start = std::chrono::high_resolution_clock::now();
	std::shared_ptr<Group> root = recursiveBuild(shapes, 0, shapes.size());

	auto stop = std::chrono::high_resolution_clock::now();

	std::cout << "Time to build BVH: " << std::chrono::duration_cast<std::chrono::milliseconds>(stop - start).count() << std::endl;

	return root;
}

std::pair<glm::dvec4, glm::dvec4> Model::mergeBounds(const std::pair<glm::dvec4, glm::dvec4> &b1, const std::pair<glm::dvec4, glm::dvec4> &b2)
{
	return std::pair<glm::dvec4, glm::dvec4>(glm::dvec4(std::min(b1.first.x, b2.first.x),
														std::min(b1.first.y, b2.first.y),
														std::min(b1.first.z, b2.first.z), 1.),
											 glm::dvec4(std::max(b1.second.x, b2.second.x),
														std::max(b1.second.y, b2.second.y),
														std::max(b1.second.z, b2.second.z), 1.));
}

std::pair<glm::dvec4, glm::dvec4> Model::mergeBounds(const std::pair<glm::dvec4, glm::dvec4> &b1, const glm::dvec4 &p)
{
	return std::pair<glm::dvec4, glm::dvec4>(glm::dvec4(std::min(b1.first.x, p.x),
														std::min(b1.first.y, p.y),
														std::min(b1.first.z, p.z), 1.),
											 glm::dvec4(std::max(b1.second.x, p.x),
														std::max(b1.second.y, p.y),
														std::max(b1.second.z, p.z), 1.));
}

Model::~Model()
{
}
