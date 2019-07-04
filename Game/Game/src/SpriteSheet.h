#pragma once

#include <fstream> 
#include <iomanip>
#include <iostream>
#include <unordered_map>
#include <string>
#include <sstream>
#include <utility>
#include <SDL.h>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

class SpriteSheet
{
public:
	SpriteSheet();
	SpriteSheet(std::string const& sp, std::string const& cp, bool json = true);
	~SpriteSheet();
	void destroy();
	//void print();

	std::vector<SDL_Rect*>* operator[] (std::string const& spriteName);

	std::string spritesPath;
	std::unordered_map<std::string, std::vector<SDL_Rect*>> gSpriteClips;

private:
	void loadText(std::string coordsPath);
	void loadJSON(std::string coordsPath);
};

