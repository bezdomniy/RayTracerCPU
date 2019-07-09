#pragma once

#include <SDL.h>
#include <gl/glew.h>
#include <SDL_opengl.h> 
#include <gl/glu.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Timer.h"
#include "Shader.h"
#include "Texture.h"

#include <stdio.h>
#include <string>

class Game
{
public:
	Game(unsigned int w_width, unsigned int w_height);
	~Game();

	static SDL_Event event;

	bool init(const char* title, int xpos, int ypos, bool fullscreen);
	void initialiseObjects();

	bool handleEvents();
	bool update();
	void render();
	void clean();

	bool running();

private:
	//The window we'll be rendering to
	SDL_Window* window;

	SDL_GLContext context;
	Timer* timer;

	GLuint VAO;
	GLuint EBO;
	GLuint VBO;

	Shader shader;
	Texture texture;

	void createVAO(float vertices[], size_t sizeV, unsigned int indices[], size_t sizeI);

	//void framebuffer_size_callback(SDL_Window* w, int width, int height);

	float timeStep = 0;

	int window_width = 0;
	int window_height = 0;

	bool isRunning;
};

