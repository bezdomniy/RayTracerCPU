#pragma once

#include "SDL.h"
#include <vector>
#include <glm/glm.hpp>
//#include <glm/gtc/matrix_transform.hpp>

#include <vector>
#include <unordered_map>
#include "Model.h"
#include "Mesh.h"
#include "SpriteSheet.h"
#include "UtilityFunctions.h"
#include "PathFinder.h"

class GameObject
{
public:
	GameObject();
	GameObject(std::string const& name);
	GameObject(std::string const& name, float x, float z);
	~GameObject();

	std::string objectName;

	//void destroy(); //TODO check 
	//void setTexture(Texture* texture);
	//void setTextureFromPath(std::string path);

	PathFinder pathFinder;

	void Draw(Shader shader, float playerRotationDegrees = 0.f);
	void nextFrame();
	void update(float timeStep);

	Model* model;
	Mesh* mesh;
	Texture* texture;

	int numberOfFrames = 0;
	int currentFrameIndex = 0;
	int frameIncrementer = 0;

	int animationSlowdown = 1;

	std::vector<Rectangle*>* spriteFrames;
	Rectangle* currentFrame;

	glm::vec3 worldSpacePosition;
	glm::vec3 velocity;
	glm::vec3 acceleration;
	glm::vec3 forward;

	void setMesh(int posX, int posZ, int width, int height, SpriteSheet* spriteSheet, std::string const& spriteName, const std::string& texturePath);

	float rotationDegrees = 0.f;

	bool collidable = true;

	Texture* gameObjectTexture;
	
private:
	//void billboard();
};

