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

	velocity << 0.f, 0.f;
	acceleration << 0.1f, 0.f;

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
	std::printf("%f\n", velocity.y());
	if (velocity.x() != 0 /*|| velocity_y != 0*/) {
		//std::printf("%f\n", velocity.x());
		spriteVector = spriteSheetPtr->operator[](runSprite);
		numberOfFrames = spriteVector->size();
	}
	else if (!onGround && velocity.y() >= 1.f /*|| velocity_y != 0*/) {
		/*std::printf("%f\n", velocity.y());*/
		spriteVector = spriteSheetPtr->operator[](jumpSprite);
		numberOfFrames = spriteVector->size();
	}
	else {
		spriteVector = spriteSheetPtr->operator[](idleSprite);
		numberOfFrames = spriteVector->size();
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
	if (playerControlled) {
		if (Game::event.type == SDL_KEYDOWN) {
			switch (Game::event.key.keysym.sym) {
			case SDLK_UP:
				if (onGround) {
					onGround = false;
					velocity.y() -= 3.f;
				}
				//else {
				//	velocity.y() = 0.f;
				//}
					
				break;
			case SDLK_DOWN:
				//if (velocity_y < 1) {
				//	velocity_y += accelerate;
				//}
				//velocity.y() = 1.0f;
				break;
			case SDLK_LEFT:
				//colliderBox->y = worldSpacePosition->y - 1;
				if (onGround && velocity.x() > -1.0f) {
					velocity.x() -= acceleration.x();
				}
				flipType = SDL_FLIP_HORIZONTAL;
				break;
			case SDLK_RIGHT:
				//colliderBox->y = worldSpacePosition->y - 1;
				if (onGround && velocity.x() < 1.0f) {
					velocity.x() += acceleration.x();
				}
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
				//velocity.y() = 0.0f;
				break;
			case SDLK_DOWN:
				//velocity.y() = 0.0f;
				break;
			case SDLK_LEFT:
				//colliderBox->y = worldSpacePosition->y + 1;
				if (Game::almostEquals(velocity.x(), 0.0f)) {
					velocity.x() = 0.0f;
				}
				if (onGround && velocity.x() > 0.0f) {
					velocity.x() -= acceleration.x();
				}
				if (onGround && velocity.x() < 0.0f) {
					velocity.x() += acceleration.x();
				}
				break;
			case SDLK_RIGHT:
				//colliderBox->y = worldSpacePosition->y + 1;
				if (Game::almostEquals(velocity.x(), 0.0f)) {
					velocity.x() = 0.0f;
				}
				if (onGround && velocity.x() > 0.0f) {
					velocity.x() -= acceleration.x();
				}
				if (onGround && velocity.x() < 0.0f) {
					velocity.x() += acceleration.x();
				}
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

	if (!fixedInPlace) {
		acceleration.y() = Game::accelerationFromGravity(acceleration.y(), velocity.y(), onGround);
	}
	if (onGround) {
		velocity.y() = 0.f;
	}
}

void Entity::move(float timeStep)
{
	//printf("%d, %d\n", velocity_x, velocity_y);
	worldSpacePosition->x += int (velocity.x() * timeStep);

	if (!onGround) {
		velocity.y() += acceleration.y();
	}

	worldSpacePosition->y += int (velocity.y() * timeStep);

	colliderBox->x = worldSpacePosition->x;
	colliderBox->y = worldSpacePosition->y;

	//onGround = false;

	if (playerControlled && checkCollisions(entityPtr)) {
		//playerControlled = false;

		onGround = true;

		worldSpacePosition->x -= 1;
		worldSpacePosition->y -= 1;
		
		velocity.y() = 0.f;
		acceleration.y() = 0.f;

		colliderBox->x = worldSpacePosition->x;
		colliderBox->y = worldSpacePosition->y;
	}
	else {
		//onGround = false;
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

void Entity::setJumpSprite(std::string name)
{
	jumpSprite = name;
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