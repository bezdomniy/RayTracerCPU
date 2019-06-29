#include "Texture.h"

Texture::Texture(SDL_Renderer* renderer, bool db)
{
	gRenderer = renderer;
	mTexture = NULL;
	mWidth = 0;
	mHeight = 0;

	debug = db;
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

void Texture::render(Entity &entity, Camera& camera)
{
	SDL_Rect cameraSpacePosition = {entity.worldSpacePosition->x - camera.worldSpacePosition->x, entity.worldSpacePosition->y - camera.worldSpacePosition->y, entity.worldSpacePosition->w, entity.worldSpacePosition->h };

	if (debug && entity.collidable) {
		SDL_Rect cameraSpacePosition = { entity.colliderBox->x - camera.worldSpacePosition->x, entity.colliderBox->y - camera.worldSpacePosition->y, entity.colliderBox->w, entity.colliderBox->h };
		SDL_SetRenderDrawColor(gRenderer, 0xFF, 0x00, 0x00, 0xFF);
		SDL_RenderDrawRect(gRenderer, &cameraSpacePosition);
		SDL_SetRenderDrawColor(gRenderer, 0x00, 0x00, 0x00, 0xFF);
	}


	SDL_RenderCopyEx(gRenderer, mTexture, entity.currentSprite, &cameraSpacePosition, entity.rotationDegrees, NULL, entity.flipType);
}

void Texture::render(Camera &camera)
{
	SDL_RenderCopy( gRenderer, mTexture, camera.worldSpacePosition, NULL);
}

int Texture::getWidth()
{
	return mWidth;
}

int Texture::getHeight()
{
	return mHeight;
}
