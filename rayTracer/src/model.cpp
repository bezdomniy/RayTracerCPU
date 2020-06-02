#include "model.h"
// #include <iostream>

Model::Model()
{
}

// #include <iostream>
// TODO use group information in model loading
// TODO use uvs and normals in model loading
Model::Model(std::string const &path)
{
	this->mesh = std::make_shared<Group>();
	std::vector<unsigned int> vertexIndices, uvIndices, normalIndices;
	std::vector<glm::dvec3> temp_vertices;
	std::vector<glm::dvec2> temp_uvs;
	std::vector<glm::dvec3> temp_normals;

	std::string line;

	std::ifstream in(path, std::ios::in);

	if (!in)
	{
		printf("Impossible to open the file !\n");
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
		else if (line.substr(0, 2) == "vt")
		{
			std::istringstream vt(line.substr(3));
			glm::dvec2 uv;
			int U, V;
			vt >> U;
			vt >> V;
			uv = glm::dvec2(U, V);
			temp_uvs.push_back(uv);
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
					uvIndex[i] = atoi(vtStr.c_str());
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
			uvIndices.push_back(uvIndex[0]);
			uvIndices.push_back(uvIndex[1]);
			uvIndices.push_back(uvIndex[2]);
			normalIndices.push_back(normalIndex[0]);
			normalIndices.push_back(normalIndex[1]);
			normalIndices.push_back(normalIndex[2]);
		}
	}

	for (unsigned int i = 0; i < vertexIndices.size(); i += 3)
	{
		// std::cout << vertexIndices[i] - 1 << " " << vertexIndices[i + 1] - 1 << " " << vertexIndices[i + 2] - 1 << "\n";

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

		mesh->addChild(nextTriangle);
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

Model::~Model()
{
}
