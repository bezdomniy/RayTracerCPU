#include "GameObject.h"


GameObject::GameObject()
{
	currentSprite = nullptr;
	flipType = SDL_FLIP_NONE;
}

//GameObject::GameObject(SDL_Rect* initialRect, int frames)
GameObject::GameObject(SpriteSheet & spriteSheet, std::string const& spriteName, int x, int y)
{
	position_x = x;
	position_y = y;
	flipType = SDL_FLIP_NONE;
	currentSprite = spriteSheet[spriteName].first;
	int frames = spriteSheet[spriteName].second;
	sprites.push_back(currentSprite);
	int width = currentSprite->w;
	for (int i = 1; i < frames; i++) {
		SDL_Rect* nextRect = new SDL_Rect({ currentSprite->x + i * currentSprite->w, currentSprite->y, currentSprite->w, currentSprite->h });
		sprites.push_back(nextRect);
	}

	numberOfFrames = frames;
}

GameObject::~GameObject()
{
}

std::pair<int, int> GameObject::getSize()
{
	return std::pair<int, int>{currentSprite->w, currentSprite->h};
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
