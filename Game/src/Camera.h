#pragma once
#include "GameObject.h"
#include "Entity.h"

class Camera :
	public GameObject
{

public:
	//using GameObject::GameObject;

	Camera();
	~Camera();

	Camera(std::string const& name, int x, int y, SDL_Renderer* renderer, Entity* player);
	void move();

private:
	Entity* playerPtr = nullptr;
};

