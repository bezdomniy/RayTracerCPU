#include "Pong.hpp"

Pong::Pong(unsigned int w_width, unsigned int w_height)
{
	gWindow = nullptr;

	gRenderer = nullptr;

	gTexture = nullptr;

	window_width = w_width;
	window_height = w_height;

	//The surface contained by the window
	//gScreenSurface = nullptr;

	isRunning = false;
}

Pong::~Pong()
{
}

bool Pong::init(const char* title, int xpos, int ypos, bool fullscreen)
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
			gRenderer = SDL_CreateRenderer(gWindow, -1, SDL_RENDERER_ACCELERATED);

			if (gRenderer == NULL) {
				printf("Renderer could not be created! SDL Error: %s\n", SDL_GetError()); success = false;
				success = false;
			}
		}
	}

	isRunning = true;

	return success;
}

bool Pong::update()
{
	//Loading success flag
	bool success = true;

	//if (gTexture != nullptr)
	//	SDL_DestroyTexture(gTexture);

	SDL_Delay(100);

	return success;
}

bool Pong::render()
{
	bool success = true;

	SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
	SDL_RenderClear(gRenderer);

	//Render red filled quad 
	SDL_Rect leftRect = { 0,0, window_width / 8, window_height / 6 };
	SDL_SetRenderDrawColor( gRenderer, 0xFF, 0x00, 0x00, 0xFF ); 
	SDL_RenderFillRect( gRenderer, &leftRect);

	SDL_Rect rightRect = { window_width - window_width / 8,0, window_width / 8, window_height / 6 };
	SDL_SetRenderDrawColor(gRenderer, 0x00, 0xFF, 0x00, 0xFF);
	SDL_RenderFillRect(gRenderer, &rightRect);

	//SDL_DestroyTexture(gTexture);

	SDL_RenderPresent(gRenderer);

	return success;
}

void Pong::clean()
{
	//Deallocate renderer
	SDL_DestroyRenderer(gRenderer);
	gRenderer = NULL;

	//Destroy window
	SDL_DestroyWindow(gWindow);
	gWindow = NULL;

	//Quit SDL subsystems
	SDL_Quit();
}

bool Pong::handleEvents()
{
	SDL_Event event;

	while (SDL_PollEvent(&event)) {
		switch (event.type) {
		case SDL_QUIT:
			isRunning = false;
			return true;
		case SDL_KEYDOWN:
			switch (event.key.keysym.sym) {
				case SDLK_UP:
					break;
				case SDLK_DOWN:
					break;
				case SDLK_LEFT:
					break;
				case SDLK_RIGHT:
					break;
				default:
					break;
			}
		default:
			return true;
		}
	}

	return false;
};

bool Pong::running()
{
	if (isRunning) {
		return true;
	}
	return false;
}

SDL_Texture* Pong::loadTexture(std::string path)
{
	SDL_Texture* newTexture = NULL;

	//Load image
	SDL_Surface* loadedSurface = IMG_Load(path.c_str());
	if (loadedSurface == NULL)
	{
		printf("Unable to load image %s! SDL Error: %s\n", path.c_str(), SDL_GetError());
	}
	else {
		newTexture = SDL_CreateTextureFromSurface(gRenderer, loadedSurface);
		if (newTexture == NULL) { 
			printf("Unable to create texture from %s! SDL Error: %s\n", path.c_str(), SDL_GetError()); 
		} 
		//Get rid of old loaded surface 
		SDL_FreeSurface(loadedSurface);
	}
	return newTexture;
}

