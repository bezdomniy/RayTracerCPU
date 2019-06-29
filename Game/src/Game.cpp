#include "Game.h"

#define DEBUG true

SDL_Event Game::event;

Game::Game(unsigned int w_width, unsigned int w_height)
{
	gWindow = nullptr;
	gRenderer = nullptr;

	//texture = NULL;
	gSpriteSheet1 = nullptr;

	timer = nullptr;

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
		timer = new Timer();
		
		gWindow = SDL_CreateWindow(title, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, window_width, window_height, SDL_WINDOW_SHOWN);
		if (gWindow == NULL)
		{
			printf("Window could not be created! SDL_Error: %s\n", SDL_GetError());
			success = false;
		}
		else
		{
			gRenderer = SDL_CreateRenderer(gWindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

			gSpriteSheet1 = new SpriteSheet("resources/platformer/adventurer.png", "resources/platformer/adventurer.json");
			entities["hero"] = Entity("hero", gSpriteSheet1, "adventurer-idle", window_width / 2, window_height / 2, &entities);
			entities["hero"].setRunSprite("adventurer-run");
			entities["ogre2"] = Entity("ogre2" , gSpriteSheet1, "adventurer-idle", window_width / 2 + 100, window_height / 2, &entities);
			entities["ogre2"].setRunSprite("adventurer-run");

			camera = Camera("camera", 0, 0, &entities["hero"]);

			gSpriteSheet2 = new SpriteSheet("resources/0x72_DungeonTilesetII_v1.1.png", "resources/tiles_list_v1.1", false);
			entities["wall1"] = Entity("wall1", gSpriteSheet2, "wall_mid", window_width / 2, (window_height / 2) + 50, &entities);
			entities["wall2"] = Entity("wall2", gSpriteSheet2, "wall_mid", (window_width / 2) - 16, (window_height / 2) + 50, &entities);
			entities["wall3"] = Entity("wall3", gSpriteSheet2, "wall_mid", (window_width / 2) + 16, (window_height / 2) + 50, &entities);
			entities["ogre3"] = Entity("ogre3", gSpriteSheet2, "ogre_idle_anim", window_width / 2 + 180, window_height / 2, &entities);
			entities["ogre3"].setRunSprite("ogre_run_anim");

			entities["hero"].setAnimationSlowdown(1);
			entities["ogre2"].setAnimationSlowdown(1);
			entities["hero"].setPlayerControlled(true);
			//player_x = entities["hero"].position_x;
			//player_y = entities["hero"].position_y;

			gSpriteSheetTexture1 = Texture(gRenderer, DEBUG);
			gSpriteSheetTexture1.loadFromFile(gSpriteSheet1->spritesPath);

			gSpriteSheetTexture2 = Texture(gRenderer, DEBUG);
			gSpriteSheetTexture2.loadFromFile(gSpriteSheet2->spritesPath);

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

	timeStep = timer->getTicks() / 5.f;

	for (auto& entity : entities) {
		entity.second.nextFrame();
		entity.second.updateVelocity();
		entity.second.move(timeStep);
	}

	camera.move();

	timer->restart();

	//SDL_Delay(100);

	return success;
}

bool Game::render()
{
	bool success = true;

	//SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
	SDL_RenderClear(gRenderer);

	gBackgroundTexture.render(camera);

	gSpriteSheetTexture1.render(entities["hero"], camera);
	gSpriteSheetTexture1.render(entities["ogre2"], camera);
	gSpriteSheetTexture2.render(entities["ogre3"], camera);

	gSpriteSheetTexture2.render(entities["wall1"], camera);
	gSpriteSheetTexture2.render(entities["wall2"], camera);
	gSpriteSheetTexture2.render(entities["wall3"], camera);

	SDL_RenderPresent(gRenderer);

	return success;
}

//void Game::drawNewRectangle(int x, int y, int width, int height) {
//	SDL_Rect rightRect = {x, y, width, height};
//	SDL_SetRenderDrawColor(gRenderer, 0x00, 0x00, 0xFF, 0xFF);
//	SDL_RenderFillRect(gRenderer, &rightRect);
//}

void Game::clean()
{
	gSpriteSheetTexture1.free();
	gBackgroundTexture.free();

	//Deallocate renderer
	SDL_DestroyRenderer(gRenderer);
	gRenderer = NULL;

	//Destroy window
	SDL_DestroyWindow(gWindow);
	gWindow = NULL;

	gSpriteSheet1->destroy();
	gSpriteSheet1 = NULL;

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

