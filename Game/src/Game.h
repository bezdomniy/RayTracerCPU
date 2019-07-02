#pragma once

#ifndef Game_h
#define Game_h

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
//#include <limits>



//const int NUMBER_OF_SPRITES = 1;

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

	//SpriteSheet* gSpriteSheet1;
	//SpriteSheet* gSpriteSheet2;

	//Texture gSpriteSheetTexture1 = NULL;
	//Texture gSpriteSheetTexture2 = NULL;

	Texture gBackgroundTexture = NULL;

	Camera camera;


	Entity player;
	//std::unordered_map<std::string, Entity> entities1;
	//std::unordered_map<std::string, Entity> entities2;

	//void drawNewRectangle(int x, int y, int width, int height);

	int window_width = 0;
	int window_height = 0;

	bool isRunning;
};

#endif // !Game_h
