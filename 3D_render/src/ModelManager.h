#pragma once

#include <map>
#include <glm/glm.hpp>

#include "Model.h"
#include "SpriteSheet.h"
#include "GameObject.h"

class ModelManager {
public:

	static std::string getFilenameFromPath(std::string const& path);
	ModelManager();
	~ModelManager();

	void addModel(char* path, std::string const& name);
	void addSpritesheet(float sheetWidth, float sheetHeight, std::string const& sp, std::string const& cp, bool json);
	void addSprite(std::string const& spriteName, std::string const& spriteSheetName, int posX, int posZ);
	void setModelPosition(std::string const& name, glm::vec3 position);
	//Model* operator[](std::string const& name);

	Model* getModel(std::string const& name);
	GameObject* getGetSprite(std::string const& name);

	int size();

	std::map<std::string, Model*> models;
	std::map<std::string, GameObject*> sprites;
	std::map<std::string, SpriteSheet*> spriteSheets;
private:

	Shader moveObjectShader;
};
