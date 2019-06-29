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
	worldSpacePosition = new SDL_Rect({ x,y, 0, 0 });
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




