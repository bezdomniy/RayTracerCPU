#pragma once

#include <fstream> 
#include <iomanip>
#include <iostream>
#include <unordered_map>
#include <string>
#include <sstream>
#include <utility>
#include <nlohmann/json.hpp>

#include "UtilityFunctions.h"

using json = nlohmann::json;



class SpriteSheet
{
public:
	SpriteSheet();
	SpriteSheet(float sheetWidth, float sheetHeight, std::string const& sp, std::string const& cp, bool json = true);
	SpriteSheet(float sheetWidth, float sheetHeight, std::string const& sp, int spriteWidth, int spriteHeight, int tilesHorizontal, int tilesVertical);

	~SpriteSheet();
	void destroy();
	//void print();

	float width = 0.f;
	float height = 0.f;

	std::vector<Rectangle*>* operator[] (std::string const& spriteName);
	Rectangle* operator[](int const spriteIndex);

	std::string spritesPath;
	std::unordered_map<std::string, std::vector<Rectangle*>> spriteClips;
	std::vector<Rectangle*> sprites;

private:
	void loadText(std::string coordsPath);
	void loadJSON(std::string coordsPath);
	void loadSequentialImage(float spriteWidth, float spriteHeight, int tilesHorizontal, int tilesVertical);
};

