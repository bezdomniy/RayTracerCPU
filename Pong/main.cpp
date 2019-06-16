#include "Pong.hpp"

Pong* game = nullptr;
//Screen dimension constants
const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

int main(int argc, char* args[])
{
	game = new Pong();
	game->init("Pong", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, true);

	while (game->running()) {
		game->handleEvents();
		game->update();
		game->render();


	}

	//Free resources and close SDL
	game->clean();

	return 0;
}