#include "GameObject.h"

GameObject::GameObject()
{
	currentSprite = nullptr;
}

GameObject::GameObject(SDL_Rect* initialRect, int frames)
{
	currentSprite = initialRect;
	sprites.push_back(initialRect);
	int width = initialRect->w;
	for (int i = 1; i < frames; i++) {
		SDL_Rect* nextRect = new SDL_Rect({ initialRect->x + i * initialRect->w, initialRect->y, initialRect->w, initialRect->h });
		sprites.push_back(nextRect);
	}

	numberOfFrames = frames;
}

GameObject::~GameObject()
{
}

void GameObject::nextFrame()
{
	if (currentSpriteIndex < numberOfFrames*(numberOfFrames * animationSlowdown) - 1)
		currentSpriteIndex++;		
	else
		currentSpriteIndex = 0;
	//std::printf("%d\n", currentSpriteIndex);
	currentSprite = sprites.at(currentSpriteIndex/(numberOfFrames * animationSlowdown));
}

void GameObject::setAnimationSlowdown(int slowdown)
{
	animationSlowdown = slowdown;
}
