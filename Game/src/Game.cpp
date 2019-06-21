#include "Game.h"

#define DEBUG false

SDL_Event Game::event;

Game::Game(unsigned int w_width, unsigned int w_height)
{
	gWindow = nullptr;

	gRenderer = nullptr;

	//texture = NULL;
	gSpriteSheet = nullptr;

	window_width = w_width;
	window_height = w_height;

	//The surface contained by the window
	//gScreenSurface = nullptr;

	isRunning = false;
}

Game::~Game()
{
}

bool Game::init(const char* title, int xpos, int ypos, bool fullscreen)
{
	//Initialization flag
	bool success = true;

	//Initialize SDL
	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
		success = false;
	}
	else
	{
		//Create window
		
		gWindow = SDL_CreateWindow(title, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, window_width, window_height, SDL_WINDOW_SHOWN);
		if (gWindow == NULL)
		{
			printf("Window could not be created! SDL_Error: %s\n", SDL_GetError());
			success = false;
		}
		else
		{
			gRenderer = SDL_CreateRenderer(gWindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

			gSpriteSheet = new SpriteSheet("resources/platformer/adventurer.png", "resources/platformer/adventurer.json");
			entities["hero"] = Entity("hero", gSpriteSheet, "adventurer-idle", window_width / 2, window_height / 2, &entities);
			entities["ogre2"] = Entity("ogre2" , gSpriteSheet, "adventurer-idle", window_width / 2 + 100, window_height / 2, &entities);

			camera = GameObject("camera", 0, 0);

			//gSpriteSheet = new SpriteSheet("resources/0x72_DungeonTilesetII_v1.1.png", "resources/tiles_list_v1.1", false);
			//gameObjects["hero"] = GameObject("hero", gSpriteSheet, "elf_m_run_anim", window_width / 2, window_height / 2, &gameObjects);
			//gameObjects["ogre2"] = GameObject("ogre2", gSpriteSheet, "ogre_idle_anim", window_width / 2 + 100, window_height / 2, &gameObjects);

			entities["hero"].setAnimationSlowdown(2);
			entities["ogre2"].setAnimationSlowdown(2);
			entities["hero"].setPlayerControlled(true);

			gSpriteSheetTexture = Texture(gRenderer, DEBUG);
			gSpriteSheetTexture.loadFromFile(gSpriteSheet->spritesPath);

			gBackgroundTexture = Texture(gRenderer, false);
			gBackgroundTexture.loadFromFile("resources/background.png");

			if (gRenderer == NULL) {
				printf("Renderer could not be created! SDL Error: %s\n", SDL_GetError()); success = false;
				success = false;
			}
		}
	}

	isRunning = true;

	return success;
}


bool Game::update()
{
	//Loading success flag
	bool success = true;

	entities["hero"].nextFrame();
	entities["ogre2"].nextFrame();

	//SDL_Delay(100);

	return success;
}

bool Game::render()
{
	bool success = true;

	//SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
	SDL_RenderClear(gRenderer);

	gBackgroundTexture.render(camera);

	gSpriteSheetTexture.render(entities["hero"]);
	gSpriteSheetTexture.render(entities["ogre2"]);



	SDL_RenderPresent(gRenderer);

	return success;
}

void Game::drawNewRectangle(int x, int y, int width, int height) {
	SDL_Rect rightRect = {x, y, width, height};
	SDL_SetRenderDrawColor(gRenderer, 0x00, 0x00, 0xFF, 0xFF);
	SDL_RenderFillRect(gRenderer, &rightRect);
}

void Game::clean()
{
	gSpriteSheetTexture.free();
	gBackgroundTexture.free();

	//Deallocate renderer
	SDL_DestroyRenderer(gRenderer);
	gRenderer = NULL;

	//Destroy window
	SDL_DestroyWindow(gWindow);
	gWindow = NULL;

	gSpriteSheet->destroy();
	gSpriteSheet = NULL;

	//Quit SDL subsystems
	IMG_Quit();
	SDL_Quit();
}

bool Game::handleEvents()
{
	SDL_PollEvent(&event);
	switch (event.type) {
	case SDL_QUIT:
		isRunning = false;
		return true;
	default:
		return true;
	}

	return false;
}


bool Game::running()
{
	if (isRunning) {
		return true;
	}
	return false;
}

