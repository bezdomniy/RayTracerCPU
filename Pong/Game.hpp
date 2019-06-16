#ifndef Game_hpp
#define Game_hpp

#include <SDL.h>
#include <SDL_image.h>
#include "Texture.hpp"
#include <stdio.h>
#include <string>

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

	Texture initialTexture = NULL;

	void newRectangle(int x, int y, int width, int height);

	int window_width = 0;
	int window_height = 0;

	bool isRunning;
};

#endif // !Pong_h
