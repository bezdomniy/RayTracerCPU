#pragma once

#include <SDL.h>
#include <SDL_image.h>

#include <string>
#include <unordered_map>

#include "Mesh.h"

struct Rectangle {
	float x;
	float y;
	float w;
	float h;
};

namespace utilityFunctions {
	Texture* loadTextureFromFile(const std::string& name, const std::string& path, std::unordered_map<std::string, Texture*>* texture_map = nullptr);
	
}