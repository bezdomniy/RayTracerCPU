#pragma once

#include <fstream> 
#include <iomanip>
#include <iostream>
#include <unordered_map>
#include <string>
#include <sstream>
#include <utility>
//#include <SDL.h>
#include "Sprite.h"
#include <nlohmann/json.hpp>

using json = nlohmann::json;

class SpriteSheet
{
public:
	SpriteSheet();
	SpriteSheet(std::string const& sp, std::string const& cp, bool json = true);
	SpriteSheet(std::string const& sp, int spriteWidth, int spriteHeight, int tilesHorizontal, int tilesVertical);

	~SpriteSheet();
	void destroy();
	//void print();

	std::vector<Sprite*>* operator[] (std::string const& spriteName);
	Sprite* SpriteSheet::operator[](int const spriteIndex);

	std::string spritesPath;
	std::unordered_map<std::string, std::vector<Sprite*>> spriteClips;
	std::vector<Sprite*> sprites;

private:
	void loadText(std::string coordsPath);
	void loadJSON(std::string coordsPath);
	void loadSequentialImage(int spriteWidth, int spriteHeight, int tilesHorizontal, int tilesVertical);
};

