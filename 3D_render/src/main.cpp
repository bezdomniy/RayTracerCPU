#ifndef GAME_H
#define GAME_H
#include "Game.h"

#include <stdio.h>
#include <unistd.h>
#include <limits.h>

Game* game = nullptr;

const int LEVEL_WIDTH = 1280;
const int LEVEL_HEIGHT = 960;

const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

int main(int argc, char* args[])
{
//    char cwd[PATH_MAX];
//    if (getcwd(cwd, sizeof(cwd)) != NULL) {
//        printf("Current working dir: %s\n", cwd);
//    } else {
//        perror("getcwd() error");
//        return 1;
//    }
//    return 0;

	game = new Game(SCREEN_WIDTH, SCREEN_HEIGHT);
	game->init("Game", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, true);

	while (game->running()) {
		game->handleEvents();
		game->update();
		game->render();
	}

	//Free resources and close SDL
	game->clean();

	return 0;
}

#endif    // GAME_H