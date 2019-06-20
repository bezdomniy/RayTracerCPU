#pragma once

#include <SDL.h>
#include <vector>
#include "SpriteSheet.h"

class GameObject
{
public:
	GameObject();
	GameObject(SpriteSheet & spriteSheet, std::string const& spriteName, int x, int y);
	~GameObject();

	void destroy(); //TODO check 

	std::vector<SDL_Rect*> sprites;
	int currentSpriteIndex = 0;
	int numberOfFrames = 0;

	int animationSlowdown = 1;

	SDL_Rect* currentSprite;
	int position_x = 0;
	int position_y = 0;
	//SDL_Point* centre;
	double rotationDegrees = 0;
	SDL_RendererFlip flipType;

	bool playerControlled = false;

	SDL_Rect* colliderBox;
	bool collision = false;

	std::pair<int, int> getSize();
	void nextFrame();
	void setAnimationSlowdown(int slowdown);
	void setPlayerControlled(bool c);
	void move();

	
private:

};
