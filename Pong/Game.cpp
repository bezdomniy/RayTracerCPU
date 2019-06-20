#include "Game.hpp"

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

			gSpriteSheet = new SpriteSheet("0x72_DungeonTilesetII_v1.1.png", "tiles_list_v1.1");

			gameObjects["hero"] = GameObject(*gSpriteSheet, "elf_f_idle_anim", window_width / 2, window_height / 2);
			gameObjects["ogre2"] = GameObject(*gSpriteSheet, "ogre_run_anim", window_width / 2 + 100, window_height / 2);
			gameObjects["ogre2"].setAnimationSlowdown(2);

			gSpriteSheetTexture = Texture(gRenderer);
			gSpriteSheetTexture.loadFromFile(gSpriteSheet->spritesPath);

			
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

	//if (texture != nullptr)
	//	texture->free();

	//initialTexture = Texture(gRenderer);
	gameObjects["hero"].nextFrame();
	gameObjects["ogre2"].nextFrame();

	//SDL_Delay(100);

	return success;
}

bool Game::render()
{
	bool success = true;

	//SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
	SDL_RenderClear(gRenderer);

	//newRectangle(0, 0, window_width/24, window_height/6);
	//newRectangle(window_width - window_width / 24, 0, window_width / 24, window_height / 6);


	gSpriteSheetTexture.render(gameObjects["hero"]);
	gSpriteSheetTexture.render(gameObjects["ogre2"]);

	//gSpriteSheetTexture.render(0, window_height / 2, gSpriteSheet["weapon_golden_sword"]);

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
	SDL_Event event;

	while (SDL_PollEvent(&event)) {
		switch (event.type) {
		case SDL_QUIT:
			isRunning = false;
			return true;
		default:
			return true;
		}
	}

		const Uint8* currentKeyStates = SDL_GetKeyboardState(NULL);
		if (currentKeyStates[SDL_SCANCODE_UP])
			gameObjects["hero"].position_y -= 1;
		if (currentKeyStates[SDL_SCANCODE_DOWN])
			gameObjects["hero"].position_y += 1;
		if (currentKeyStates[SDL_SCANCODE_LEFT])
			gameObjects["hero"].position_x -= 1;
		if (currentKeyStates[SDL_SCANCODE_RIGHT])
			gameObjects["hero"].position_x += 1;
		if (currentKeyStates[SDL_SCANCODE_D])
			gameObjects["hero"].flipType = SDL_FLIP_VERTICAL;
		if (currentKeyStates[SDL_SCANCODE_C])
			gameObjects["hero"].flipType = SDL_FLIP_NONE;
		if (currentKeyStates[SDL_SCANCODE_Z])
			gameObjects["hero"].rotationDegrees -= 60;
		if (currentKeyStates[SDL_SCANCODE_X])
			gameObjects["hero"].rotationDegrees += 60;
	

	return false;
};

bool Game::running()
{
	if (isRunning) {
		return true;
	}
	return false;
}



