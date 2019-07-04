#pragma once
#include "GameObject.h"
//#include "Camera.h"
//#include "Collider.h"

class Entity :
	public GameObject
{
public:
	//using GameObject::GameObject;

	Entity();
	~Entity();
	Entity(std::string const& name, SpriteSheet* spriteSheet, std::string const& spriteName, int x, int y, SDL_Renderer* renderer, std::unordered_map<std::string, Entity>* entities);

	bool playerControlled = false;

	SDL_Rect* colliderBox = nullptr;

	std::unordered_map<std::string, Entity>* entityPtr;

	std::pair<int, int> getSize();

	void nextFrame();
	void setAnimationSlowdown(int slowdown);
	void setPlayerControlled(bool c);

	void updateVelocity();
	void move(float timeStep);

	void setIdleSprite(std::string name);
	void setRunSprite(std::string name);
	void setJumpSprite(std::string name);
	void setAttackSprite(std::string name);
	//void setIdleSprite(std::string);

	bool fixedInPlace = true;

	bool onGround = false;

	void render(GameObject& camera);


	bool checkCollisions(std::unordered_map<std::string, Entity>* objectMap);

	int currentSpriteIndex = 0;
	size_t numberOfFrames = 0;
	int animationSlowdown = 1;

	std::vector<SDL_Rect*>* spriteVector;
	SpriteSheet* spriteSheetPtr;
	SDL_Rect* currentSprite;

private:
	std::string idleSprite;
	std::string runSprite;
	std::string jumpSprite;
	std::string attackSprite;
	//std::string runSprite;
	
};

