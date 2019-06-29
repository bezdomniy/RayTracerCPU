#pragma once

#include <SDL.h>
#include <vector>
#include <unordered_map>
#include "SpriteSheet.h"
//#include "Game.h"

class GameObject
{
public:
	GameObject();
	GameObject(std::string const& name, int x, int y);
	~GameObject();

	std::string objectName;


	void destroy(); //TODO check 

	SDL_Rect* worldSpacePosition = nullptr;

	float velocity_x = 0;
	float velocity_y = 0;
	//SDL_Point* centre;
	double rotationDegrees = 0;
	SDL_RendererFlip flipType;

	bool collidable = true;


	
//private:

};

