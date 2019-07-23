#include "Player.h"
#include "Game.h"

Player::Player()
{
}

Player::~Player()
{
}

Player::Player(std::string const& name, float x, float z) : GameObject(name, x, z)
{
}

void Player::updateVelocity()
{

	if (velocity.x < maxVelocity && velocity.x > -maxVelocity) {
		velocity.x += (Game::kb[SDL_SCANCODE_A] - Game::kb[SDL_SCANCODE_D]) * acceleration.x;
	}
	if (velocity.z < maxVelocity && velocity.z > -maxVelocity) {
		velocity.z += (Game::kb[SDL_SCANCODE_W] - Game::kb[SDL_SCANCODE_S]) * acceleration.z;
	}

	rotationDegrees += (-Game::kb[SDL_SCANCODE_LEFT] + Game::kb[SDL_SCANCODE_RIGHT]) * 4 ;

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

	//std::cout << "x: " << velocity.x << " , z: " << velocity.z << "\n";

}
