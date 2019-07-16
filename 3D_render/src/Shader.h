#pragma once
#include <gl/glew.h>
#include <SDL_opengl.h> 
#include <gl/glu.h>

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <fstream>
#include <sstream>
#include <string>
#include <iostream>

class Shader {
public:
	GLuint id;

	Shader();
	Shader(const char* vertexPath, const char* fragmentPath = nullptr, const char* geometryPath = nullptr);
	void use();
	void setModel(glm::mat4 viewMatrix);
	void setView(glm::mat4 viewMatrix);
	void setProjection(glm::mat4 projectionMatrix);

	void setVector3(std::string const& name, glm::vec3 vector);

private:
	void checkCompileErrors(GLuint shader, std::string type);
};