#pragma once


#include <SDL.h>
#include <SDL_image.h>
#include "Texture.h"
#include "SpriteSheet.h"
#include "GameObject.h"
#include "Entity.h"
#include "Camera.h"
#include "Timer.h"
#include "ObjectManager.h"

#include <stdio.h>
#include <string>

class Game
{
public:
	Game(unsigned int w_width, unsigned int w_height);
	~Game();

	static SDL_Event event;
	static float accelerationFromGravity(float force, float velocity, bool onGround);
	static float accelerationFromJump(float force);

	//template <typename T>
	static bool almostEquals(float a, float b);

	bool init(const char* title, int xpos, int ypos, bool fullscreen);

	bool handleEvents();
	bool update();
	bool render();
	void clean();

	bool running();

private:
	//The window we'll be rendering to
	SDL_Window* gWindow;

	SDL_Renderer* gRenderer;

	Timer* timer;

	float timeStep = 0;

	ObjectManager* objectManager = nullptr;

	Texture gBackgroundTexture = NULL;

	Camera camera;


	Entity player;

	int window_width = 0;
	int window_height = 0;

	bool isRunning;
};

