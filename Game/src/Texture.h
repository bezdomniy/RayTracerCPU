#pragma once

#include <string>
#include <SDL.h>
#include <SDL_image.h>
#include "GameObject.h"

class Texture
{
public:
	Texture(SDL_Renderer* renderer, bool db = false);
	~Texture();

	bool loadFromFile(std::string path);
	void free();
	void render(GameObject gameObject);

	int getWidth();
	int getHeight();

private:
	SDL_Texture* mTexture;
	SDL_Renderer* gRenderer;

	int mWidth;
	int mHeight;

	bool debug;
};

