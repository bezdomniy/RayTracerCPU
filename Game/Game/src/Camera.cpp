#include "Camera.h"

Camera::Camera()
{
}

Camera::~Camera()
{
}

Camera::Camera(std::string const& name, int x, int y, SDL_Renderer* renderer, Entity* player) : GameObject(name, x, y, renderer) {
	playerPtr = player;


	worldSpacePosition->w = 640;
	worldSpacePosition->h = 480;
	//worldSpacePosition = new SDL_Rect({ x,y, 640, 480 });
}

void Camera::move()
{
	if (playerPtr->worldSpacePosition->x - 320 > 0 && playerPtr->worldSpacePosition->x + 320 < 1280)
		worldSpacePosition->x = playerPtr->worldSpacePosition->x - 320;

	if (playerPtr->worldSpacePosition->y - 240 > 0 && playerPtr->worldSpacePosition->y + 240 < 960)
		worldSpacePosition->y = playerPtr->worldSpacePosition->y - 240;
}
