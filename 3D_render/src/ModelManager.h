#pragma once

#include <map>
#include <glm/glm.hpp>

#include "Model.h"

class ModelManager {
public:
	ModelManager();
	~ModelManager();

	void addModel(char* path, std::string const& name);
	void setModelPosition(std::string const& name, glm::vec3 position);
	Model* operator[](std::string const& name);
	int size();

	std::map<std::string, Model*> models;
private:

	Shader moveObjectShader;
};
