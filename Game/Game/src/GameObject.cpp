#include "GameObject.h"


GameObject::GameObject()
{
	flipType = SDL_FLIP_NONE;
	//centre = nullptr;
}

//GameObject::GameObject(SDL_Rect* initialRect, int frames)
GameObject::GameObject(std::string const& name, int x, int y, SDL_Renderer* renderer)
{
	objectName = name;
	worldSpacePosition = new SDL_Rect({ x,y, 0, 0 });
	flipType = SDL_FLIP_NONE;

	velocity << 0.f, 0.f;
	acceleration << 0.f, 0.f;

	collidable = false;

	rendererPtr = renderer;
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

void GameObject::setTexture(Texture* texture)
{
	gameObjectTexture = texture;
}

void GameObject::setTextureFromPath(std::string path)
{
	Texture* newTexture = new Texture(rendererPtr);
	newTexture->loadFromFile(path);
	setTexture(newTexture);
}

void GameObject::render()
{
	SDL_RenderCopy(rendererPtr, gameObjectTexture->get(), worldSpacePosition, NULL);
}




