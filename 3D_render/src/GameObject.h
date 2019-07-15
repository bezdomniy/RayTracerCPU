#pragma once

#include <SDL.h>
#include <glm/glm.hpp>
//#include <glm/gtc/matrix_transform.hpp>

#include <vector>
#include <unordered_map>
#include "Mesh.h"

class GameObject
{
public:
	GameObject();
	GameObject(std::string const& name, float x, float z);
	~GameObject();

	std::string objectName;

	//void destroy(); //TODO check 
	//void setTexture(Texture* texture);
	//void setTextureFromPath(std::string path);

	void render();

	Mesh* mesh;

	glm::vec3 worldSpacePosition;
	glm::vec3 velocity;
	glm::vec3 acceleration;
	glm::vec3 forward;

	float rotationDegrees = 0.f;

	bool collidable = true;

	Texture* gameObjectTexture;
	
//private:

};

