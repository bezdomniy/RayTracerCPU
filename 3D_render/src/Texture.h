#pragma once

#include <string>
#include <SDL.h>
#include <SDL_image.h>
#include <gl/glew.h>

class Texture
{
public:
	Texture();
	Texture(std::string const& path);
	~Texture();

	GLuint id = 0;

	void loadFromFile(std::string const& path);

	GLuint get();

	int getWidth();
	int getHeight();

private:

	int mWidth;
	int mHeight;

};

