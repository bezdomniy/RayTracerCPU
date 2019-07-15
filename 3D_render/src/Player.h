#pragma once
#include "GameObject.h"
#include <glm/gtx/rotate_vector.hpp>


class Player :
	public GameObject
{
public:
	Player();
	~Player();

	Player(std::string const& name, int x, int z);

	void updateVelocity();
	
	void move(float timeStep);

private:
	float maxVelocity = 0.00005f;

};