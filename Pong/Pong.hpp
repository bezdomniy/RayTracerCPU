#ifndef Pong_hpp
#define Pong_hpp

#include <SDL.h>
#include <SDL_image.h>
#include <stdio.h>
#include <string>

class Pong
{
public:
	Pong(unsigned int window_width, unsigned int window_height);
	~Pong();

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

	SDL_Texture* gTexture;

	//Loads individual image as texture 
	SDL_Texture* loadTexture( std::string path );

	int window_width = 0;
	int window_height = 0;

	bool isRunning;
};

#endif // !Pong_h
