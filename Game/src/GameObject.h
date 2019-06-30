#pragma once

#include <SDL.h>
#include <vector>
#include <unordered_map>
#include <Eigen/Dense>
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

	Eigen::Vector2f velocity;
	Eigen::Vector2f acceleration;

	//SDL_Point* centre;
	double rotationDegrees = 0.0;
	SDL_RendererFlip flipType;

	bool collidable = true;


	
//private:

};

