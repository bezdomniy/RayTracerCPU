#include "GameObject.h"


GameObject::GameObject()
{
}

GameObject::GameObject(std::string const& name)
{
	objectName = name;
	//worldSpacePosition = glm::vec3(x, -0.5f, z);

	velocity = glm::vec3(0.f, 0.f, 0.f);
	forward = glm::vec3(0.f, 0.f, 1.f);
	acceleration = glm::vec3(0.025f, 0.f, 0.025f);

	collidable = false;
}

GameObject::GameObject(std::string const& name, float x, float z)
{
	objectName = name;
	worldSpacePosition = glm::vec3(x, -0.5f, z);
	pathFinder = PathFinder(&worldSpacePosition);

	velocity = glm::vec3(0.f, 0.f, 0.f);
	forward = glm::vec3(1.f, 1.f, 1.f);
	acceleration = glm::vec3(0.025f, 0.f, 0.025f);

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

void GameObject::Draw(Shader shader, float playerRotationDegrees)
{
	//std::cout << mesh->vertices[0].TexCoords.x << " " << mesh->vertices[0].TexCoords.y <<  "\n";


	mesh->Draw(shader, texture, worldSpacePosition, currentFrameIndex, playerRotationDegrees);
	rotationDegrees = playerRotationDegrees;
}

void GameObject::nextFrame()
{
	if (frameIncrementer < numberOfFrames * (numberOfFrames * animationSlowdown) - 1) {
		frameIncrementer++;
	}
	else {
		frameIncrementer = 0;
	}
	
	currentFrameIndex = (frameIncrementer / (numberOfFrames * animationSlowdown));
	currentFrame = spriteFrames->operator[](currentFrameIndex);


}

void GameObject::update(float timeStep)
{
	nextFrame();

	velocity = pathFinder.findPathVector(rotationDegrees);
	worldSpacePosition += (velocity * timeStep/50.f);
}

void GameObject::setMesh(int posX, int posZ, int width, int height, SpriteSheet* spriteSheet, std::string const& spriteName, const std::string& texturePath)
{
	spriteFrames = spriteSheet->operator[](spriteName);
	numberOfFrames = spriteSheet->operator[](spriteName)->size();
	currentFrame = spriteFrames->operator[](0);

	std::vector<Vertex> vertices;
	std::vector<unsigned int> indices;
	std::vector<Texture*> textures;

	Vertex v0;
	Vertex v1;
	Vertex v2;
	Vertex v3;

	v0.Position = { glm::vec3(0, 0, (float)-width / 2) };
	v1.Position= { glm::vec3(0, 0, (float)width/2) };
	v2.Position = { glm::vec3(0, height, (float)width/2) };
	v3.Position = { glm::vec3(0, height, (float)-width / 2) };

	worldSpacePosition = glm::vec3(posX, 0.f, posZ);

	v0.Normal = glm::vec3(1.f, 0.f, 0.f);
	v1.Normal = glm::vec3(1.f, 0.f, 0.f);
	v2.Normal = glm::vec3(1.f, 0.f, 0.f);
	v3.Normal = glm::vec3(1.f, 0.f, 0.f);


	v2.TexCoords = glm::vec2(currentFrame->x, currentFrame->y);
	v3.TexCoords = glm::vec2(currentFrame->x + currentFrame->w, currentFrame->y);
	v0.TexCoords = glm::vec2(currentFrame->x + currentFrame->w, currentFrame->y + currentFrame->h);
	v1.TexCoords = glm::vec2(currentFrame->x, currentFrame->y + currentFrame->h);

	//v0.TexCoords = glm::vec2(0.0f, 0.0f);
	//v1.TexCoords = glm::vec2(0.5f, 0.0f);
	//v2.TexCoords = glm::vec2(0.5f, 0.5f);
	//v3.TexCoords = glm::vec2(0.0f, 0.5f);

	vertices.push_back(v0);
	vertices.push_back(v1);
	vertices.push_back(v2);
	vertices.push_back(v3);

	indices = { 0, 1, 3,
				1, 3, 2 };

	mesh = new Mesh(vertices, indices, textures);
	mesh->type = objectName;
	texture = utilityFunctions::loadTextureFromFile(objectName, texturePath);

}




