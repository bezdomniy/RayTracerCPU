#include "GameObject.h"


GameObject::GameObject()
{
	flipType = SDL_FLIP_NONE;
	//centre = nullptr;
}

//GameObject::GameObject(SDL_Rect* initialRect, int frames)
GameObject::GameObject(std::string const& name, int x, int y)
{
	objectName = name;
	position_x = x;
	position_y = y;
	flipType = SDL_FLIP_NONE;

	collidable = false;
	//currentSprite = new SDL_Rect({ position_x, position_y, 640, 480 });
}

GameObject::~GameObject()
{
	//destroy();
}

void GameObject::destroy()
{
	//delete colliderBox;
	//colliderBox = nullptr;
}




