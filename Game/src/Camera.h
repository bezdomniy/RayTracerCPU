#pragma once
#include "GameObject.h"
#include "Entity.h"

class Camera :
	public GameObject
{

public:
	Camera();
	~Camera();

	Camera(std::string const& name, int x, int y, Entity* player);
	void move();

private:
	Entity* playerPtr = nullptr;
};

