#ifndef Game_hpp
#define Game_hpp

#include <SDL.h>
#include <SDL_image.h>
#include "Texture.hpp"
#include "SpriteSheet.h"
#include "GameObject.h"
#include <stdio.h>
#include <string>

//const int NUMBER_OF_SPRITES = 1;

class Game
{
public:
	Game(unsigned int window_width, unsigned int window_height);
	~Game();

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

	SpriteSheet gSpriteSheet;

	std::unordered_map<std::string, GameObject> gameObjects;

	Texture gSpriteSheetTexture = NULL;

	void drawNewRectangle(int x, int y, int width, int height);

	int window_width = 0;
	int window_height = 0;

	bool isRunning;
};

#endif // !Pong_h
