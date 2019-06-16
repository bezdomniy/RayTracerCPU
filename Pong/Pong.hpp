#ifndef Pong_hpp
#define Pong_hpp

#include <SDL.h>
#include <SDL_image.h>
#include <stdio.h>
#include <string>

class Pong
{
public:
	Pong();
	~Pong();

	bool init(const char* title, int xpos, int ypos, int width, int height, bool fullscreen);

	bool handleEvents();
	bool update();
	bool render();
	void clean();

	bool running();

private:
	//The window we'll be rendering to
	SDL_Window* gWindow;

	SDL_Renderer* gRenderer;

	SDL_Texture* gTexture;

	//Loads individual image as texture 
	SDL_Texture* loadTexture( std::string path );

	bool isRunning;
};

#endif // !Pong_h
