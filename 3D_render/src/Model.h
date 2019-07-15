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

class Model
{
public:
	Model();
	Model(char* path);
	~Model();

	std::vector<Texture*> textures_loaded;
	
	void Draw(Shader shader);
private:
	std::vector<Mesh> meshes;
	std::string directory;

	void loadModel(std::string path);
	void processNode(aiNode* node, const aiScene* scene);
	Mesh processMesh(aiMesh* mesh, const aiScene* scene);

	std::vector<Texture*> loadMaterialsTexture(aiMaterial* mat, aiTextureType type, std::string typeName);
	void loadColors(aiMaterial* mat, Vertex& vertex);
	unsigned int TextureFromFile(const char* path, const std::string& directory/*, bool gamma*/);
};

