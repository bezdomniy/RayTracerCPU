#include "Player.h"
#include "Game.h"

Player::Player()
{
}

Player::~Player()
{
}

Player::Player(std::string const& name, int x, int z) : GameObject(name, x, z)
{
}

void Player::updateVelocity()
{
	//if (Game::event.type == SDL_KEYDOWN) {
	//	switch (Game::event.key.keysym.sym) {
	//	case SDLK_UP:
	//		if (velocity.z < maxVelocity)
	//			velocity.z += acceleration.z ;
	//		break;
	//	case SDLK_DOWN:
	//		if (velocity.z > -maxVelocity)
	//			velocity.z -= acceleration.z ;
	//		break;

	//	case SDLK_LEFT:
	//		if (velocity.x < maxVelocity)
	//			velocity.x += acceleration.x;
	//		break;
	//	case SDLK_RIGHT:

	//		if (velocity.x > -maxVelocity)
	//			velocity.x -= acceleration.x;
	//		break;
	//	case SDLK_z:
	//		rotationDegrees -= 2.f;
	//		break;
	//	case SDLK_x:
	//		rotationDegrees += 2.f;
	//		break;
	//	default:
	//		break;
	//	}
	//}

	//if (Game::event.type == SDL_KEYUP) {
	//	switch (Game::event.key.keysym.sym) {
	//	case SDLK_UP:
	//		if (Game::almostEquals(velocity.z, 0.0f)) {
	//			velocity.z = 0.0f;
	//		}
	//		if (velocity.z > 0.0f) {
	//			velocity.z -= acceleration.z ;
	//		}
	//		if (velocity.z < 0.0f) {
	//			velocity.z += acceleration.z ;
	//		}
	//		break;
	//	case SDLK_DOWN:
	//		if (Game::almostEquals(velocity.z, 0.0f)) {
	//			velocity.z = 0.0f;
	//		}
	//		if (velocity.z > 0.0f) {
	//			velocity.z -= acceleration.z;
	//		}
	//		if (velocity.z < 0.0f) {
	//			velocity.z += acceleration.z;
	//		}
	//		break;
	//	case SDLK_LEFT:
	//		if (Game::almostEquals(velocity.x, 0.0f)) {
	//			velocity.x = 0.0f;
	//		}
	//		if (velocity.x > 0.0f) {
	//			velocity.x -= acceleration.x;
	//		}
	//		if (velocity.x < 0.0f) {
	//			velocity.x += acceleration.x;
	//		}
	//		break;
	//	case SDLK_RIGHT:
	//		//colliderBox->y = worldSpacePosition->y + 1;
	//		if (Game::almostEquals(velocity.x, 0.0f)) {
	//			velocity.x = 0.0f;
	//		}
	//		if (velocity.x > 0.0f) {
	//			velocity.x -= acceleration.x;
	//		}
	//		if (velocity.x < 0.0f) {
	//			velocity.x += acceleration.x;
	//		}
	//		break;
	//	default:
	//		break;
	//	}
	//}

	if (velocity.x < maxVelocity && velocity.x > -maxVelocity) {
		velocity.x += (Game::kb[SDL_SCANCODE_A] - Game::kb[SDL_SCANCODE_D]) * acceleration.x;
	}
	if (velocity.z < maxVelocity && velocity.z > -maxVelocity) {
		velocity.z += (Game::kb[SDL_SCANCODE_W] - Game::kb[SDL_SCANCODE_S]) * acceleration.z;
	}

	rotationDegrees += (-Game::kb[SDL_SCANCODE_LEFT] + Game::kb[SDL_SCANCODE_RIGHT]) ;

	if (!Game::kb[SDL_SCANCODE_A] && !Game::kb[SDL_SCANCODE_D]) {
		if (Game::almostEquals(velocity.x, 0.0f)) {
			velocity.x = 0.0f;
		}
		else if (velocity.x > 0.0f) {
			velocity.x -= acceleration.x;
		}
		else if (velocity.x < 0.0f) {
			velocity.x += acceleration.x;
		}
	}
	if (!Game::kb[SDL_SCANCODE_W] && !Game::kb[SDL_SCANCODE_S]) {
		if (Game::almostEquals(velocity.z, 0.0f)) {
			velocity.z = 0.0f;
		}
		else if (velocity.z > 0.0f) {
			velocity.z -= acceleration.z;
		}
		else if (velocity.z < 0.0f) {
			velocity.z += acceleration.z;
		}
	}
}

void Player::move(float timeStep)
{
	forward = glm::rotate(velocity, glm::radians(rotationDegrees), glm::vec3(0.0f, -1.0f, 0.0f));
	//std::cout << "x: " << forward.x << " , z: " << forward.z << "\n";


	worldSpacePosition += (forward * timeStep);

	std::cout << "x: " << velocity.x << " , z: " << velocity.z << "\n";

}
