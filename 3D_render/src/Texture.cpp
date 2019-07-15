#include "Texture.h"

TextureClass::TextureClass()
{

}

TextureClass::TextureClass(std::string const& path)
{
	mWidth = 0;
	mHeight = 0;
	loadFromFile(path);
}

TextureClass::~TextureClass()
{
}

void TextureClass::loadFromFile(std::string const& path)
{
	//Load image
	//IMG_Init(IMG_INIT_JPG);
	SDL_Surface* loadedSurface = IMG_Load(path.c_str());
	if (loadedSurface == nullptr)
	{
		printf("Unable to load image %s! SDL Error: %s\n", path.c_str(), SDL_GetError());

	}
	else {
		mWidth = loadedSurface->w;
		mHeight = loadedSurface->h;

		glGenTextures(1, &id);
		glBindTexture(GL_TEXTURE_2D, id);

		// set the texture wrapping/filtering options (on the currently bound texture object)
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		int Mode = GL_RGB;

		if (loadedSurface->format->BytesPerPixel == 4) {
			Mode = GL_RGBA;
		}

		glTexImage2D(GL_TEXTURE_2D, 0, Mode, loadedSurface->w, loadedSurface->h, 0, Mode, GL_UNSIGNED_BYTE, loadedSurface->pixels);
		glGenerateMipmap(GL_TEXTURE_2D);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	}

	//Get rid of old loaded surface 
	SDL_FreeSurface(loadedSurface);
	//IMG_Quit();
}

GLuint TextureClass::get()
{
	return id;
}

int TextureClass::getWidth()
{
	return mWidth;
}

int TextureClass::getHeight()
{
	return mHeight;
}
