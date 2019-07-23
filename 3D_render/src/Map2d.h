#pragma once

#include <SDL.h>
#include <SDL_image.h>

#include <vector>
#include <string>
#include <unordered_map>

#include "Shader.h"
#include "UtilityFunctions.h"

class Map2d
{
public:
	Map2d();
	Map2d(char* path);
	~Map2d();

	Mesh addWall(int posX, int posZ, int alignment, bool facingOut, const std::string& texturePath, bool addToMeshes = true);
	Mesh addCorner(int posX, int posZ, int alignment, bool facingOut, const std::string& texturePath, bool addToMeshes = true);
	Mesh addBlock(int posX, int posZ, const std::string& texturePath, bool addToMeshes = true);
	Mesh addFloor(int posX, int posZ, const std::string& texturePath);

	void Map2d::loadMap(int grid[], size_t gridSize, int width, int height, const std::string& wallTexturePath, const std::string& floorTexturePath);
	
	void Draw(Shader shader);
private:
	std::vector<Mesh> meshes;
	std::vector<glm::vec3> worldSpacePositions;
	std::unordered_map<std::string, Texture*> texture_map;
	std::string directory;

};

