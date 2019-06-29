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
	//delete collider;
}

Entity::Entity(std::string const& name, SpriteSheet* spriteSheet, std::string const& spriteName, int x, int y, std::unordered_map<std::string, Entity>* entities) : GameObject(name, x, y)
{
	entityPtr = entities;

	idleSprite = spriteName;

	spriteSheetPtr = spriteSheet;
	spriteVector = spriteSheetPtr->operator[](idleSprite);

	numberOfFrames = spriteVector->size();

	//std::cout << numberOfFrames << "\n";

	currentSprite = spriteVector->at(currentSpriteIndex);

	worldSpacePosition = new SDL_Rect({ x,y, currentSprite->w, currentSprite->h });

	collidable = true;

	if (collidable) {
		colliderBox = new SDL_Rect({ x,y, worldSpacePosition->w, worldSpacePosition->h });
	}

}

std::pair<int, int> Entity::getSize()
{
	return std::pair<int, int>{worldSpacePosition->w, worldSpacePosition->h};
}


void Entity::nextFrame()
{
	if (velocity_x != 0 || velocity_y != 0) {
		spriteVector = spriteSheetPtr->operator[](runSprite);
	}
	else {
		spriteVector = spriteSheetPtr->operator[](idleSprite);
	}
	if (currentSpriteIndex < numberOfFrames * (numberOfFrames * animationSlowdown) - 1)
		//if (currentSpriteIndex < numberOfFrames - 1)
		currentSpriteIndex++;
	else
		currentSpriteIndex = 0;

	currentSprite = spriteVector->at(currentSpriteIndex / (numberOfFrames * animationSlowdown));
	worldSpacePosition->w = currentSprite->w;
	worldSpacePosition->h = currentSprite->h;

	colliderBox->w = worldSpacePosition->w;
	colliderBox->h = worldSpacePosition->h;
	//currentSprite = spriteVector->at(currentSpriteIndex);

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

void Entity::move(float timeStep)
{
	//printf("%d, %d\n", velocity_x, velocity_y);
	worldSpacePosition->x += int (velocity_x * timeStep);
	worldSpacePosition->y += int (velocity_y * timeStep);

	colliderBox->x = worldSpacePosition->x;
	colliderBox->y = worldSpacePosition->y;

	const short int BUMPBACK = 1;

	if (playerControlled && checkCollisions(entityPtr)) {
		//playerControlled = false;
		worldSpacePosition->x -= int (velocity_x * timeStep);
		worldSpacePosition->y -= int (velocity_y * timeStep);

		colliderBox->x = worldSpacePosition->x;
		colliderBox->y = worldSpacePosition->y;
	}
}

void Entity::setIdleSprite(std::string name)
{
	idleSprite = name;
}

void Entity::setRunSprite(std::string name)
{
	runSprite = name;
}

void Entity::setAttackSprite(std::string name)
{
	attackSprite = name;
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