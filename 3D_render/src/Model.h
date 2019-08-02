#pragma once

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <SDL.h>
#include <SDL_image.h>

#include <vector>
#include <string>

#include "Shader.h"
#include "Mesh.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class Model
{
public:
	Model();
	Model(std::string const& path);
	~Model();

	std::vector<Texture*> textures_loaded;
	void setPosition(glm::vec3 newPos);
	glm::vec3 worldPosition;
	void Draw(Shader shader);
private:
	std::vector<Mesh> meshes;
	std::string directory;



	void loadModel(std::string const& path);
	void processNode(aiNode* node, const aiScene* scene);
	Mesh processMesh(aiMesh* mesh, const aiScene* scene);

	std::vector<Texture*> loadMaterialsTexture(aiMaterial* mat, aiTextureType type, std::string typeName);
	void loadColors(aiMaterial* mat, Vertex& vertex);
	unsigned int TextureFromFile(std::string const& path, const std::string& directory/*, bool gamma*/);
};
