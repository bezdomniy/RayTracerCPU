#include "Texture.hpp"

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

void Texture::render(GameObject gameObject)
{
	//Set rendering space and render to screen 
	SDL_Rect renderQuad = { gameObject.position_x, gameObject.position_y, gameObject.getSize().first, gameObject.getSize().second };

	if (gameObject.currentSprite != NULL) {
		renderQuad.w = gameObject.currentSprite->w;
		renderQuad.h = gameObject.currentSprite->h;
	}

	SDL_RenderCopyEx( gRenderer, mTexture, gameObject.currentSprite, &renderQuad, gameObject.rotationDegrees, NULL, gameObject.flipType);
}

int Texture::getWidth()
{
	return mWidth;
}

int Texture::getHeight()
{
	return mHeight;
}
