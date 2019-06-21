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


	int position_x = 0;
	int position_y = 0;

	int velocity_x = 0;
	int velocity_y = 0;
	//SDL_Point* centre;
	double rotationDegrees = 0;
	SDL_RendererFlip flipType;

	bool collidable = true;


	
//private:

};

