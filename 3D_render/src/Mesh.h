#pragma once

#include <glm/glm.hpp>
#include <vector>

#include <string>
#include "Shader.h"

struct Vertex {
	glm::vec3 Position;
	glm::vec3 Normal;
	glm::vec4 Colour;
	glm::vec2 TexCoords;

	// tangent
	//glm::vec3 Tangent;
	//// bitangent
	//glm::vec3 Bitangent;
};

struct Texture {
	unsigned int id;
	std::string type;
	std::string path;
};

class Mesh {
public:
	/*  Mesh Data  */
	std::string type;
	std::vector<Vertex> vertices;
	std::vector<unsigned int> indices;
	std::vector<Texture*> textures;
	/*  Functions  */
	Mesh();
	Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<Texture*> textures);
	void Draw(Shader shader);
	void Draw(Shader shader, Texture* texture);

	Mesh combine(Mesh& other);
private:
	/*  Render data  */
	unsigned int VAO, VBO, EBO;
	/*  Functions    */
	void setupMesh();
};