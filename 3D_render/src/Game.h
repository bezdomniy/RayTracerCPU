#pragma once

#include "SDL.h"
#include <GL/glew.h>
#include <SDL_opengl.h> 
#include <GL/glu.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Timer.h"
#include "Shader.h"
#include "ModelManager.h"
//#include "Model.h"
#include "Map2d.h"
#include "Player.h"

#include <iostream>
#include <string>

class Game
{
public:
	Game(unsigned int w_width, unsigned int w_height);
	~Game();

	static const Uint8* kb; 
	static SDL_Event event;

	static bool almostEquals(float a, float b);

	bool init(const char* title, int xpos, int ypos, bool fullscreen);
	void initialiseObjects();
	void addPlayer(float x, float y);

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
	ModelManager pointLights;
	ModelManager sprites;
	//Model donutTop;
	Map2d map;

	GLuint VAO;
	GLuint EBO;
	GLuint VBO;

	Shader shader;
	Shader lightShader;
	Player player;
	
	//TextureClass texture;

	//void framebuffer_size_callback(SDL_Window* w, int width, int height);

	float timeStep = 0.f;

	int window_width = 0;
	int window_height = 0;

	bool isRunning;

	bool startAnim = false;
};

