#include "GameObject.h"


GameObject::GameObject()
{
	currentSprite = nullptr;
	colliderBox = nullptr;
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
	colliderBox = new SDL_Rect({ position_x - 1, position_y - 1, currentSprite->w + 2, currentSprite->h + 2 });

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
	//destroy();
}

void GameObject::destroy()
{
	delete colliderBox;
	//colliderBox = nullptr;
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
	const int BUMPBACK = 3;
	if (playerControlled) {
		const Uint8* currentKeyStates = SDL_GetKeyboardState(NULL);
		if (currentKeyStates[SDL_SCANCODE_UP]) {
			if (collision) {
				collision = false;
				position_y += BUMPBACK;
				colliderBox->y += BUMPBACK;
			}
			else {
				position_y -= 1;
				colliderBox->y -= 1;
			}

		}

		if (currentKeyStates[SDL_SCANCODE_DOWN]) {
			if (collision) {
				collision = false;
				position_y -= BUMPBACK;
				colliderBox->y -= BUMPBACK;
			}
			else {
				position_y += 1;
				colliderBox->y += 1;
			}
		}

		if (currentKeyStates[SDL_SCANCODE_LEFT]) {
			if (collision) {
				collision = false;
				position_x += BUMPBACK;
				colliderBox->x += BUMPBACK;
			}
			else {
				flipType = SDL_FLIP_HORIZONTAL;
				position_x -= 1;
				colliderBox->x -= 1;
			}
		}
		if (currentKeyStates[SDL_SCANCODE_RIGHT]) {
			if (collision) {
				collision = false;
				position_x -= BUMPBACK;
				colliderBox->x -= BUMPBACK;
			}
			else {
				position_x += 1;
				colliderBox->x += 1;
				flipType = SDL_FLIP_NONE;
			}
		}
		if (currentKeyStates[SDL_SCANCODE_Z]) {
			rotationDegrees -= 60;
		}

		if (currentKeyStates[SDL_SCANCODE_X]) {
			rotationDegrees += 60;
		}

	}
}


