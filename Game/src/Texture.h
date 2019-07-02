#pragma once

#include <string>
#include <SDL.h>
#include <SDL_image.h>
//#include "GameObject.h"
//#include "Entity.h"
//#include "Camera.h"

class Texture
{
public:
	Texture(SDL_Renderer* renderer);
	~Texture();

	bool loadFromFile(std::string path);
	void free();
	SDL_Texture* get();

	int getWidth();
	int getHeight();

private:
	SDL_Texture* mTexture;
	SDL_Renderer* gRenderer;

	int mWidth;
	int mHeight;

};

