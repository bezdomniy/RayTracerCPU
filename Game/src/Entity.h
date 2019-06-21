#pragma once
#include "GameObject.h"

class Entity :
	public GameObject
{
public:
	//using GameObject::~GameObject;

	Entity();
	~Entity();
	Entity(std::string const& name, SpriteSheet* spriteSheet, std::string const& spriteName, int x, int y, std::unordered_map<std::string, Entity>* entities);

	using GameObject::destroy;

	using GameObject::objectName;

	using GameObject::position_x;
	using GameObject::position_y;

	using GameObject::velocity_x;
	using GameObject::velocity_y;

	using GameObject::rotationDegrees;
	using GameObject::flipType;

	bool playerControlled = false;

	SDL_Rect* colliderBox;

	std::unordered_map<std::string, Entity>* entityPtr;

	std::pair<int, int> getSize();
	void nextFrame();
	void setAnimationSlowdown(int slowdown);
	void setPlayerControlled(bool c);

	void updateVelocity();
	void move();

	bool checkCollisions(std::unordered_map<std::string, Entity>* objectMap);


	int currentSpriteIndex = 0;
	size_t numberOfFrames = 0;
	int animationSlowdown = 1;



	std::vector<SDL_Rect*>* spriteVector;
	SpriteSheet* spriteSheetPtr;
	SDL_Rect* currentSprite;
};

