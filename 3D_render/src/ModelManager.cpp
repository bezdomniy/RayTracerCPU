#include "ModelManager.h"

std::string ModelManager::getFilenameFromPath(std::string const& path)
{
	std::size_t last_slash = path.find_last_of("/");
	return path.substr(last_slash + 1);
}

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

void ModelManager::addSpritesheet(float sheetWidth, float sheetHeight, std::string const& sp, std::string const& cp, bool json)
{
	spriteSheets[ModelManager::getFilenameFromPath(sp)] = new SpriteSheet(sheetWidth, sheetHeight, sp, cp, json);
}

void ModelManager::addSprite(std::string const& spriteName, std::string const& spriteSheetName, int posX, int posZ)
{
	sprites[spriteName] = new GameObject(spriteName);
	sprites[spriteName]->setMesh(posX, posZ, 1, 1, spriteSheets[spriteSheetName], spriteName, spriteSheets[spriteSheetName]->spritesPath);
}

void ModelManager::setModelPosition(std::string const& name, glm::vec3 position)
{
	models[name]->setPosition(position);
}

Model* ModelManager::getModel(std::string const& name)
{
	return models[name];
}

GameObject* ModelManager::getGetSprite(std::string const& name)
{
	return sprites[name];
}

//Model* ModelManager::operator[](std::string const& name)
//{
//	return models[name];
//}

int ModelManager::size()
{
	return models.size();
}
