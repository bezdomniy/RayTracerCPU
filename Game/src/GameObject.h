#pragma once

#include <SDL.h>
#include <vector>
#include <unordered_map>
#include <Eigen/Dense>
#include "SpriteSheet.h"
#include "Texture.h"
//#include "Game.h"

class GameObject
{
public:
	GameObject();
	GameObject(std::string const& name, int x, int y, SDL_Renderer* renderer);
	~GameObject();

	std::string objectName;

	void destroy(); //TODO check 

	void setTexture(Texture* texture);
	void setTextureFromPath(std::string path);

	void render();
	//void render(Camera& camera);

	SDL_Rect* worldSpacePosition = nullptr;

	Eigen::Vector2f velocity;
	Eigen::Vector2f acceleration;

	//SDL_Point* centre;
	double rotationDegrees = 0.0;
	SDL_RendererFlip flipType;

	bool collidable = true;

	Texture* gameObjectTexture;
	SDL_Renderer* rendererPtr;


	
//private:

};

