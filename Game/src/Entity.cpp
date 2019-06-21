#include "entity.h"
#include "Game.h"

Entity::Entity()
{
	currentSprite = nullptr;
	spriteVector = nullptr;
	colliderBox = nullptr;
	entityPtr = nullptr;
	spriteSheetPtr = nullptr;
	flipType = SDL_FLIP_NONE;
	//centre = nullptr;
}

Entity::~Entity()
{
}

Entity::Entity(std::string const& name, SpriteSheet* spriteSheet, std::string const& spriteName, int x, int y, std::unordered_map<std::string, Entity>* entities)
{
	objectName = name;
	entityPtr = entities;
	position_x = x;
	position_y = y;
	flipType = SDL_FLIP_NONE;

	if (objectName != "camera") {
		spriteSheetPtr = spriteSheet;
		spriteVector = spriteSheetPtr->operator[](spriteName);

		numberOfFrames = spriteVector->size();
		//std::cout << numberOfFrames << "\n";

		currentSprite = spriteVector->at(currentSpriteIndex);
		colliderBox = new SDL_Rect({ position_x - 1, position_y - 1, currentSprite->w + 2, currentSprite->h + 2 }); //TODO update to remove border buffer
	}
	else {
		collidable = false;
		currentSprite = new SDL_Rect({ position_x, position_y, 640, 480 });
	}
}

std::pair<int, int> Entity::getSize()
{
	return std::pair<int, int>{currentSprite->w, currentSprite->h};
}

void Entity::nextFrame()
{
	if (velocity_x != 0 || velocity_y != 0) {
		spriteVector = spriteSheetPtr->operator[]("adventurer-run");
	}
	else {
		spriteVector = spriteSheetPtr->operator[]("adventurer-idle");
	}
	if (currentSpriteIndex < numberOfFrames * (numberOfFrames * animationSlowdown) - 1)
		//if (currentSpriteIndex < numberOfFrames - 1)
		currentSpriteIndex++;
	else
		currentSpriteIndex = 0;

	currentSprite = spriteVector->at(currentSpriteIndex / (numberOfFrames * animationSlowdown));
	//currentSprite = spriteVector->at(currentSpriteIndex);

	updateVelocity();
	move();
}

void Entity::setAnimationSlowdown(int slowdown)
{
	animationSlowdown = slowdown;
}

void Entity::setPlayerControlled(bool c)
{
	playerControlled = c;
}

void Entity::updateVelocity()
{
	//const short int BUMPBACK = 1;
	if (playerControlled) {
		if (Game::event.type == SDL_KEYDOWN) {
			switch (Game::event.key.keysym.sym) {
			case SDLK_UP:
				velocity_y = -1;
				break;
			case SDLK_DOWN:
				velocity_y = 1;
				break;
			case SDLK_LEFT:
				velocity_x = -1;
				flipType = SDL_FLIP_HORIZONTAL;
				break;
			case SDLK_RIGHT:
				velocity_x = 1;
				flipType = SDL_FLIP_NONE;
				break;
			case SDLK_z:
				rotationDegrees -= 60;
				break;
			case SDLK_x:
				rotationDegrees += 60;
				break;
			default:
				break;
			}
			//playerControlled = true;
		}

		if (Game::event.type == SDL_KEYUP) {
			switch (Game::event.key.keysym.sym) {
			case SDLK_UP:
				velocity_y = 0;
				break;
			case SDLK_DOWN:
				velocity_y = 0;
				break;
			case SDLK_LEFT:
				velocity_x = 0;
				break;
			case SDLK_RIGHT:
				velocity_x = 0;
				break;
			default:
				break;
				//case SDLK_z:
				//	rotationDegrees -= 60;
				//case SDLK_x:
				//	rotationDegrees += 60;
			}
			//playerControlled = true;
		}
	}
}

void Entity::move()
{
	//printf("%d, %d\n", velocity_x, velocity_y);
	position_x += velocity_x;
	position_y += velocity_y;

	colliderBox->x = position_x;
	colliderBox->y = position_y;

	const short int BUMPBACK = 1;

	if (playerControlled && checkCollisions(entityPtr)) {
		//playerControlled = false;
		position_x -= velocity_x;
		position_y -= velocity_y;

		colliderBox->x = position_x;
		colliderBox->y = position_y;
	}
}

bool Entity::checkCollisions(std::unordered_map<std::string, Entity>* objectMap)
{
	for (auto& object : *objectMap) {
		if (objectName != object.second.objectName && object.second.collidable) {
			if (SDL_HasIntersection(colliderBox, object.second.colliderBox)) {
				return true;
			}
		}
	}
}