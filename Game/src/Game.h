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
#include <stdio.h>
#include <string>



//const int NUMBER_OF_SPRITES = 1;

class Game
{
public:
	Game(unsigned int w_width, unsigned int w_height);
	~Game();

	static SDL_Event event;

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

	SpriteSheet* gSpriteSheet1;
	SpriteSheet* gSpriteSheet2;

	Texture gSpriteSheetTexture1 = NULL;
	Texture gSpriteSheetTexture2 = NULL;

	Texture gBackgroundTexture = NULL;

	Camera camera;



	std::unordered_map<std::string, Entity> entities;

	//void drawNewRectangle(int x, int y, int width, int height);

	int window_width = 0;
	int window_height = 0;

	bool isRunning;
};

#endif // !Game_h
