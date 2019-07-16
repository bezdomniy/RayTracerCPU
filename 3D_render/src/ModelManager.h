#pragma once

#include <unordered_map>
#include <glm/glm.hpp>

#include "Model.h"

class ModelManager {
public:
	ModelManager();
	~ModelManager();

	void addModel(char* path, std::string const& name);
	void setModelPosition(std::string const& name, glm::vec3 position);
	Model* operator[](std::string const& name);
private:
	std::unordered_map<std::string, Model*> models;
	Shader moveObjectShader;
};
