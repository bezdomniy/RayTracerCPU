#include "Game.h"
//#include "Box2D/Box2D.h"



SDL_Event Game::event;

Game::Game(unsigned int w_width, unsigned int w_height)
{
	gWindow = nullptr;
	gRenderer = nullptr;

	//texture = NULL;
	//gSpriteSheet1 = nullptr;

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

float Game::accelerationFromGravity(float acceleration, float velocity, bool onGround)
{
	//return 0.f;
	const float terminalVelocity = 2.f;
	const float gravityConstant = 1.f / 60.f / 2.f;

	if (!onGround && velocity <= terminalVelocity) {
		acceleration += gravityConstant;
	}
	else if (acceleration > 0) {
		acceleration -= gravityConstant;
	}

	return acceleration;
}

//float Game::accelerationFromJump(float force)
//{
//	const float terminalVelocity = 5.f;
//
//	if (force < terminalVelocity) {
//		force += 0.1f;
//	}
//
//	return force;
//}

//template <typename T>
bool Game::almostEquals(float a, float b)
{
	return std::abs(a - b) < std::numeric_limits<float>::epsilon();
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

			objectManager = new ObjectManager(gRenderer);
			objectManager->addSpriteSheet("../../../resources/platformer/adventurer.png", "../../../resources/platformer/adventurer.json");
			objectManager->addSpriteSheet("../../../resources/0x72_DungeonTilesetII_v1.1.png", "../../../resources/tiles_list_v1.1", false);
			for (auto& spriteSheet : objectManager->spriteSheets) {
				objectManager->addTexture(gRenderer, spriteSheet.second);
			}

			player = Entity("hero", objectManager->getSpriteSheet("adventurer-idle"), "adventurer-idle", 0 , window_height / 2, gRenderer, &objectManager->tiles);
			player.setTexture(objectManager->texturesByName["adventurer.png"]);
			player.setRunSprite("adventurer-run");
			player.setJumpSprite("adventurer-fall");
			player.fixedInPlace = false;
			player.setAnimationSlowdown(1);
			player.setPlayerControlled(true);

			//entities1["ogre2"] = Entity("ogre2" , gSpriteSheet1, "adventurer-idle", window_width / 2 + 100, window_height / 2, &entities1);
			//entities1["ogre2"].setRunSprite("adventurer-run");

			camera = Camera("camera", 0, 500, gRenderer, &player);


			objectManager->loadMapFromFile("../../../resources/platformer/map.mp", "0x72_DungeonTilesetII_v1.1.png");

			camera.setTextureFromPath("../../../resources/background.png");

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

	player.nextFrame();
	player.updateVelocity();
	player.move(timeStep);

	//for (auto& entity : objectManager->entities) {
	//	entity.second.nextFrame();
	//	entity.second.updateVelocity();
	//	entity.second.move(timeStep);
	//}

	//for (auto& tiles : objectManager->tiles) {
	//	tiles.second.nextFrame();
	//	tiles.second.updateVelocity();
	//	tiles.second.move(timeStep);
	//}

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

	camera.render();
	
	for (auto& entity : objectManager->entities) {
		entity.second.render(camera);
	}
	for (auto& tile : objectManager->tiles) {
		tile.second.render(camera);
	}

	player.render(camera);

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
	//gSpriteSheetTexture1.free();
	//gBackgroundTexture.free();

	//Deallocate renderer
	SDL_DestroyRenderer(gRenderer);
	gRenderer = NULL;

	//Destroy window
	SDL_DestroyWindow(gWindow);
	gWindow = NULL;

	//gSpriteSheet1->destroy();
	//gSpriteSheet1 = NULL;

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

