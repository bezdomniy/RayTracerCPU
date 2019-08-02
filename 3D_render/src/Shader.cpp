// #pragma once
#include "Shader.h"


Shader::Shader()
{
}

Shader::Shader(const char* vertexPath, const char* fragmentPath, const char* geometryPath)
{
	std::string vertexCode;
	std::string fragmentCode;
	std::string geometryCode;

	std::ifstream fileStream;
	// ensure ifstream objects can throw exceptions:
	fileStream.exceptions(std::ifstream::failbit | std::ifstream::badbit);

	try
	{
		// open files
		fileStream.open(vertexPath);
		std::stringstream stringStream;
		// read file's buffer contents into streams
		stringStream << fileStream.rdbuf();
		// close file handlers
		fileStream.close();
		// convert stream into string
		vertexCode = stringStream.str();
		stringStream.str(std::string());

		if (fragmentPath != nullptr)
		{
			fileStream.open(fragmentPath);
			stringStream << fileStream.rdbuf();
			fileStream.close();
			fragmentCode = stringStream.str();
			stringStream.str(std::string());
		}
		// if geometry shader path is present, also load a geometry shader
		if (geometryPath != nullptr)
		{
			fileStream.open(geometryPath);
			stringStream << fileStream.rdbuf();
			fileStream.close();
			geometryCode = stringStream.str();
			stringStream.str(std::string());
		}
	}
	catch (std::ifstream::failure e)
	{
		std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << std::endl;
	}

	const char* vShaderCode = vertexCode.c_str();
	// 2. compile shaders
	unsigned int vertex;
	// vertex shader
	vertex = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertex, 1, &vShaderCode, NULL);
	glCompileShader(vertex);
	checkCompileErrors(vertex, "VERTEX");

	// fragment Shader
	unsigned int fragment;
	if (fragmentPath != nullptr)
	{
		const char* fShaderCode = fragmentCode.c_str();
		fragment = glCreateShader(GL_FRAGMENT_SHADER);
		glShaderSource(fragment, 1, &fShaderCode, NULL);
		glCompileShader(fragment);
		checkCompileErrors(fragment, "FRAGMENT");
	}
	// if geometry shader is given, compile geometry shader
	unsigned int geometry;
	if (geometryPath != nullptr)
	{
		const char* gShaderCode = geometryCode.c_str();
		geometry = glCreateShader(GL_GEOMETRY_SHADER);
		glShaderSource(geometry, 1, &gShaderCode, NULL);
		glCompileShader(geometry);
		checkCompileErrors(geometry, "GEOMETRY");
	}
	// shader Program
	id = glCreateProgram();
	glAttachShader(id, vertex);
	if (fragmentPath != nullptr)
		glAttachShader(id, fragment);
	if (geometryPath != nullptr)
		glAttachShader(id, geometry);
	glLinkProgram(id);
	checkCompileErrors(id, "PROGRAM");
	// delete the shaders as they're linked into our program now and no longer necessery
	glDeleteShader(vertex);
	if (fragmentPath != nullptr)
		glDeleteShader(fragment);
	if (geometryPath != nullptr)
		glDeleteShader(geometry);
}

void Shader::use()
{
	glUseProgram(id);
}

void Shader::setModel(glm::mat4 modelMatrix)
{
	glUseProgram(id);
	unsigned int modelLoc = glGetUniformLocation(id, "model");
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modelMatrix));
}

void Shader::setView(glm::mat4 viewMatrix)
{
	glUseProgram(id);
	unsigned int viewLoc = glGetUniformLocation(id, "view");
	glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(viewMatrix));

}

void Shader::setProjection(glm::mat4 projectionMatrix)
{
	glUseProgram(id);
	unsigned int projectionLoc = glGetUniformLocation(id, "projection");
	glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projectionMatrix));
}

void Shader::setFloat(std::string const& name, glm::float32 value)
{
	glUseProgram(id);
	unsigned int attribute = glGetUniformLocation(id, name.c_str());
	glUniform1f(attribute, value);
}

void Shader::setInt(std::string const& name, glm::int32 value)
{
	glUseProgram(id);
	unsigned int attribute = glGetUniformLocation(id, name.c_str());
	glUniform1i(attribute, value);
}

void Shader::setVector2(std::string const& name, glm::vec2 vector)
{
	glUseProgram(id);
	unsigned int attribute = glGetUniformLocation(id, name.c_str());
	glUniform2f(attribute, vector.x, vector.y);
}

void Shader::setVector3(std::string const& name, glm::vec3 vector)
{
	glUseProgram(id);
	unsigned int attribute = glGetUniformLocation(id, name.c_str());
	glUniform3f(attribute, vector.x, vector.y, vector.z);
}

void Shader::checkCompileErrors(GLuint shader, std::string type)
{
	GLint success;
	GLchar infoLog[1024];
	if (type != "PROGRAM")
	{
		glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
		if (!success)
		{
			glGetShaderInfoLog(shader, 1024, NULL, infoLog);
			std::cout << "ERROR::SHADER_COMPILATION_ERROR of type: " << type << "\n" << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
		}
	}
	else
	{
		glGetProgramiv(shader, GL_LINK_STATUS, &success);
		if (!success)
		{
			glGetProgramInfoLog(shader, 1024, NULL, infoLog);
			std::cout << "ERROR::PROGRAM_LINKING_ERROR of type: " << type << "\n" << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
		}
	}
}
