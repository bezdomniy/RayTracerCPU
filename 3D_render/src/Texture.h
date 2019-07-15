#pragma once

#include <string>
#include <SDL.h>
#include <SDL_image.h>
#include <gl/glew.h>

class TextureClass
{
public:
	TextureClass();
	TextureClass(std::string const& path);
	~TextureClass();

	GLuint id = 0;
	std::string type;

	void loadFromFile(std::string const& path);

	GLuint get();

	int getWidth();
	int getHeight();

private:

	int mWidth;
	int mHeight;

};

