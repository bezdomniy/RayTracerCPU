#pragma once
#include <SDL.h>
#include <unordered_map>
#include <vector>
#include "Texture.h"
#include "Entity.h"
#include "SpriteSheet.h"

class ObjectManager
{
public:
	ObjectManager(SDL_Renderer* renderer);
	~ObjectManager();

	static std::string getFilenameFromPath(std::string const& path);

	void loadMapFromFile(std::string const path, std::string textureName);
	void addSpriteSheet(std::string const& sp, std::string const& cp, bool json = true);
	void addTexture(SDL_Renderer *renderer, SpriteSheet *spriteSheet);
	SpriteSheet* getSpriteSheet(std::string const spriteName);
	//std::unordered_map<std::string, SpriteSheet*>* getSpriteSheets();
	
	std::unordered_map<std::string, Entity> entities;
	std::unordered_map<std::string, Entity> tiles;

	std::vector<std::string> indexToSprite;

	int gridWidth = 16;
	int gridHeight = 16;

	std::unordered_map<std::string, SpriteSheet*> spriteSheets;

	std::unordered_map<std::string, Texture*> texturesByName;
	std::vector<Texture*> texturesByIndex;

	SDL_Renderer* rendererPtr;
};

