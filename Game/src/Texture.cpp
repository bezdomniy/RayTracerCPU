#include "Texture.h"

Texture::Texture(SDL_Renderer* renderer)
{
	gRenderer = renderer;
	mTexture = NULL;
	mWidth = 0;
	mHeight = 0;

}

Texture::~Texture()
{
	free();
}

bool Texture::loadFromFile(std::string path)
{
	free();

	SDL_Texture* newTexture = NULL;

	//Load image
	SDL_Surface* loadedSurface = IMG_Load(path.c_str());
	if (loadedSurface == NULL)
	{
		printf("Unable to load image %s! SDL Error: %s\n", path.c_str(), SDL_GetError());

	}
	else {
		newTexture = SDL_CreateTextureFromSurface(gRenderer, loadedSurface);
		if (newTexture == NULL) {
			printf("Unable to create texture from %s! SDL Error: %s\n", path.c_str(), SDL_GetError());
		}
		else {
			mWidth = loadedSurface->w;
			mHeight = loadedSurface->h;
		}
		//Get rid of old loaded surface 
		
		SDL_FreeSurface(loadedSurface);
	}

	mTexture = newTexture;
	return mTexture != NULL;
}

void Texture::free()
{
	if (mTexture != NULL) {
		SDL_DestroyTexture(mTexture);
		mTexture = NULL;
		mWidth = 0;
		mHeight = 0;
	}
}

SDL_Texture* Texture::get()
{
	return mTexture;
}

int Texture::getWidth()
{
	return mWidth;
}

int Texture::getHeight()
{
	return mHeight;
}
