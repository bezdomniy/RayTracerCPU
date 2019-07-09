#pragma once
#include <gl/glew.h>
#include <SDL_opengl.h> 
#include <gl/glu.h>

#include <glm/glm.hpp>
#include <fstream>
#include <sstream>
#include <string>
#include <iostream>

class Shader {
public:
	GLuint id;

	Shader();
	Shader(const char* vertexPath, const char* fragmentPath, const char* geometryPath = nullptr);
	void use();

private:
	void checkCompileErrors(GLuint shader, std::string type);
};