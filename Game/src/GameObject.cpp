#include "GameObject.h"


GameObject::GameObject()
{
	currentSprite = nullptr;
	flipType = SDL_FLIP_NONE;
	//centre = nullptr;
}

//GameObject::GameObject(SDL_Rect* initialRect, int frames)
GameObject::GameObject(SpriteSheet & spriteSheet, std::string const& spriteName, int x, int y)
{
	position_x = x;
	position_y = y;
	flipType = SDL_FLIP_NONE;
	currentSprite = spriteSheet[spriteName].first;
	//centre = new SDL_Point({ position_x + currentSprite->w / 2, position_y + currentSprite->h / 2 });

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

	currentSprite = sprites.at(currentSpriteIndex/(numberOfFrames * animationSlowdown));

	move();
}

void GameObject::setAnimationSlowdown(int slowdown)
{
	animationSlowdown = slowdown;
}

void GameObject::setPlayerControlled(bool c)
{
	playerControlled = c;
}

void GameObject::move()
{
	if (playerControlled) {
		const Uint8* currentKeyStates = SDL_GetKeyboardState(NULL);
		if (currentKeyStates[SDL_SCANCODE_UP])
			position_y -= 1;
		if (currentKeyStates[SDL_SCANCODE_DOWN])
			position_y += 1;
		if (currentKeyStates[SDL_SCANCODE_LEFT]) {
			flipType = SDL_FLIP_HORIZONTAL;
			position_x -= 1;
		}
		if (currentKeyStates[SDL_SCANCODE_RIGHT]) {
			position_x += 1;
			flipType = SDL_FLIP_NONE;
		}
		if (currentKeyStates[SDL_SCANCODE_Z])
			rotationDegrees -= 60;
		if (currentKeyStates[SDL_SCANCODE_X])
			rotationDegrees += 60;
	}
}
