#include "ModelManager.h"

ModelManager::ModelManager()
{
	//moveObjectShader = Shader("../../../src/shaders/moveVertex.vert");
}

ModelManager::~ModelManager()
{
}

void ModelManager::addModel(char* path, std::string const& name)
{
	models[name] = new Model(path);
}

void ModelManager::setModelPosition(std::string const& name, glm::vec3 position)
{
	models[name]->setPosition(position);
}

Model* ModelManager::operator[](std::string const& name)
{
	return models[name];
}
