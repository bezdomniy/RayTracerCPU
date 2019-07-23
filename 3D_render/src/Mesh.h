#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
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

	glm::vec3 ambient;
	glm::vec3 diffuse;
	glm::vec3 specular;
	float shininess;
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
	~Mesh();
	void Draw(Shader shader, glm::vec3 worldPosition);
	void Draw(Shader shader, Texture* texture, glm::vec3 worldPosition, int currentFrame = 0, float playerRotationDegrees = 0.f);

	float centreX = 0;
	float centreY = 0;
	float centreZ = 0;

	Mesh combine(Mesh& other);
	void changeTexture(glm::vec2 newTexCoords[]);

	/*  Render data  */
	unsigned int VAO, VBO, EBO;
private:

	/*  Functions    */
	void setupMesh();
};