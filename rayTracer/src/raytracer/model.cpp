#include "model.h"
#include <chrono>
#include <iostream>

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
		mesh = std::make_shared<Group>(triangles);
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

std::shared_ptr<Group> Model::recursiveBuild(std::vector<std::shared_ptr<Shape>> &shapes, uint32_t start, uint32_t end)
{
	std::shared_ptr<Group> node = std::make_shared<Group>();

	int nShapes = end - start;

	if (nShapes == 1)
	{
		for (int i = start; i < end; ++i)
			node->addChild(shapes.at(i));
		return node;
	}
	else
	{
		std::pair<glm::dvec4, glm::dvec4> centroidBounds;
		//for (const auto &shape : shapes)
		for (auto it = shapes.begin() + start; it != shapes.begin() + end; ++it)
			centroidBounds = mergeBounds(centroidBounds, (*it)->bounds());

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
			mid = (start + end) / 2;
			std::nth_element(&shapes[start], &shapes[mid],
							 &shapes[end - 1] + 1,
							 [splitDimension](const std::shared_ptr<Shape> &a, const std::shared_ptr<Shape> &b) {
								 return a->boundsCentroid()[splitDimension] < b->boundsCentroid()[splitDimension];
							 });

			// double pmid = (centroidBounds.first[splitDimension] + centroidBounds.second[splitDimension]) / 2;
			// std::shared_ptr<Shape> *midPtr =
			// 	std::partition(&shapes[start], &shapes[end - 1] + 1,
			// 				   [splitDimension, pmid](const std::shared_ptr<Shape> &pi) {
			// 					   return pi->boundsCentroid()[splitDimension] < pmid;
			// 				   });
			// mid = midPtr - &shapes[0];
			// if (mid != start && mid != end)
			// 	break;

			std::shared_ptr<Shape> leftChild = recursiveBuild(shapes, start, mid);
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

	std::cout << "Time to build BVH: " << std::chrono::duration_cast<std::chrono::seconds>(stop - start).count() << std::endl;

	return root;
}

std::pair<glm::dvec4, glm::dvec4> Model::mergeBounds(const std::pair<glm::dvec4, glm::dvec4> b1, const std::pair<glm::dvec4, glm::dvec4> b2)
{
	return std::pair<glm::dvec4, glm::dvec4>(glm::dvec4(std::min(b1.first.x, b2.first.x),
														std::min(b1.first.y, b2.first.y),
														std::min(b1.first.z, b2.first.z), 1.),
											 glm::dvec4(std::max(b1.second.x, b2.second.x),
														std::max(b1.second.y, b2.second.y),
														std::max(b1.second.z, b2.second.z), 1.));
}

Model::~Model()
{
}
