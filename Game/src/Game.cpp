#include "Game.h"
//#include "Box2D/Box2D.h"

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

float Game::accelerationFromGravity(float acceleration, float velocity, bool onGround)
{
	const float terminalVelocity = 2.f;
	const float gravityConstant = 1.f / 60.f / 2.f;

	if (!onGround && acceleration <= terminalVelocity) {
		acceleration += gravityConstant;
	}
	else if (acceleration > 0) {
		acceleration -= gravityConstant;
	}

	return acceleration;
}

float Game::accelerationFromJump(float force)
{
	const float terminalVelocity = 5.f;

	if (force < terminalVelocity) {
		force += 0.1f;
	}

	return force;
}

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

			gSpriteSheet1 = new SpriteSheet("resources/platformer/adventurer.png", "resources/platformer/adventurer.json");
			entities1["hero"] = Entity("hero", gSpriteSheet1, "adventurer-idle", window_width / 2 , window_height / 2 + 500, &entities2);
			entities1["hero"].setRunSprite("adventurer-run");
			entities1["hero"].setJumpSprite("adventurer-fall");
			entities1["hero"].fixedInPlace = false;

			//entities1["ogre2"] = Entity("ogre2" , gSpriteSheet1, "adventurer-idle", window_width / 2 + 100, window_height / 2, &entities1);
			//entities1["ogre2"].setRunSprite("adventurer-run");

			camera = Camera("camera", 0, 500, &entities1["hero"]);

			gSpriteSheet2 = new SpriteSheet("resources/0x72_DungeonTilesetII_v1.1.png", "resources/tiles_list_v1.1", false);

			for (int i = 0; i < 20; i++) {
				entities2["wall"+ std::to_string(i)] = Entity("wall" + std::to_string(i), gSpriteSheet2, "wall_mid", (window_width / 2) + 16*i, (window_height / 2) + 550, &entities1);
			}
			for (int i = 0; i < 20; i++) {
				entities2["wall" + std::to_string(i+20)] = Entity("wall" + std::to_string(i + 20), gSpriteSheet2, "wall_mid", (window_width / 2) + 360 + 16 * i, (window_height / 2) + 450, &entities1);
			}
			//entities2["ogre3"] = Entity("ogre3", gSpriteSheet2, "ogre_idle_anim", (window_width / 2) + 100, (window_height / 2) - 100, &entities1);
			//entities2["ogre3"].setRunSprite("ogre_run_anim");

			entities1["hero"].setAnimationSlowdown(1);
			//entities1["ogre2"].setAnimationSlowdown(1);
			entities1["hero"].setPlayerControlled(true);
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

	for (auto& entity : entities1) {
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

	gSpriteSheetTexture1.render(entities1["hero"], camera);
	//gSpriteSheetTexture1.render(entities1["ogre2"], camera);

	for (auto& entity : entities2) {
		gSpriteSheetTexture2.render(entity.second, camera);
	}



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

