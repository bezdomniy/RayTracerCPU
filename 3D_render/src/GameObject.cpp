#include "GameObject.h"


GameObject::GameObject()
{
}

GameObject::GameObject(std::string const& name, float x, float z)
{
	objectName = name;
	worldSpacePosition = glm::vec3(x, -0.5f, z);

	velocity = glm::vec3(0.f, 0.f, 0.f);
	forward = glm::vec3(1.f, 1.f, 1.f);
	acceleration = glm::vec3(0.000025f, 0.f, 0.000025f);

	collidable = false;
}

GameObject::~GameObject()
{
}

//void GameObject::setTexture(Texture* texture)
//{
//	gameObjectTexture = texture;
//}
//
//void GameObject::setTextureFromPath(std::string path)
//{
//	Texture* newTexture = new Texture(rendererPtr);
//	newTexture->loadFromFile(path);
//	setTexture(newTexture);
//}

void GameObject::render()
{
	
}




