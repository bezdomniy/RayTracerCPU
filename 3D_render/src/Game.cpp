#include "Game.h"
#include <iostream>

SDL_Event Game::event;
const Uint8* Game::kb = SDL_GetKeyboardState(NULL);


Game::Game(unsigned int w_width, unsigned int w_height)
{
	window = nullptr;
	//context = nullptr;
	timer = nullptr;

	window_width = w_width;
	window_height = w_height;


	isRunning = false;
}

Game::~Game()
{
}


bool Game::almostEquals(float a, float b)
{
	return std::abs(a - b) < std::numeric_limits<float>::epsilon();
}

bool Game::init(const char* title, int xpos, int ypos, bool fullscreen)
{
	//Initialization flag
	bool success = true;

	//Use OpenGL 3.3 core 
	SDL_GL_SetAttribute( SDL_GL_CONTEXT_MAJOR_VERSION, 3 ); 
	SDL_GL_SetAttribute( SDL_GL_CONTEXT_MINOR_VERSION, 3 ); 
	SDL_GL_SetAttribute( SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE );
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

	//SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

	//Initialize SDL
	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
		success = false;
	}
	else
	{
		timer = new Timer();
		
		window = SDL_CreateWindow(title, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, window_width, window_height, SDL_WINDOW_RESIZABLE | SDL_WINDOW_OPENGL);
		if (window == NULL)
		{
			printf("Window could not be created! SDL_Error: %s\n", SDL_GetError());
			success = false;
		}
		else
		{
			context = SDL_GL_CreateContext(window);

			if (context == NULL) {
				printf("Context could not be created! SDL Error: %s\n", SDL_GetError()); success = false;
				success = false;
			}
			else {
				//Initialize GLEW 
				glewExperimental = GL_TRUE; 
				GLenum glewError = glewInit(); 
				if( glewError != GLEW_OK ) { 
					printf( "Error initializing GLEW! %s\n", glewGetErrorString( glewError ) ); 
				}

				if (SDL_GL_SetSwapInterval(1) < 0) {
					printf("Warning: Unable to set VSync! SDL Error: %s\n", SDL_GetError());
				}

				shader = Shader("../../../src/shaders/vertexShader.vert","../../../src/shaders/fragmentShader.frag");
				lightShader = Shader("../../../src/shaders/vertexShader.vert", "../../../src/shaders/fragmentColourShader.frag");
				
				
				glEnable(GL_DEPTH_TEST);
				initialiseObjects();

			}
		}
	}


	glViewport(0, 0, window_width, window_height);

	isRunning = true;

	return success;
}

void Game::initialiseObjects()
{
	pointLights.addModel("../../../resources/blender/WhiteCube.obj", "light1");
	pointLights.setModelPosition("light1", glm::vec3(17.f, 1.5f, 17.f));

	pointLights.addModel("../../../resources/blender/WhiteCube.obj", "light2");
	pointLights.setModelPosition("light2", glm::vec3(2.f, 1.5f, 2.f));

	//pointLights.addModel("../../../resources/blender/WhiteCube.obj", "light3");
	//pointLights.setModelPosition("light3", glm::vec3(14.f, 1.5f, 2.f));
	//donutTop = Model("../../../resources/blender/donutTop.ply");

	//int grid[] = {  5,3,3,3,3,3,3,3,3,6,
	//				1,0,0,0,0,0,0,0,0,2,
	//				1,0,0,0,0,9,0,0,0,2,
	//				1,0,0,0,0,0,0,0,0,2,
	//				1,0,9,0,0,0,0,0,0,2,
	//				1,0,0,0,0,0,0,0,0,2,
	//				1,0,0,0,0,0,9,0,0,2,
	//				1,0,0,9,0,0,0,0,0,2,
	//				1,0,0,0,0,0,0,0,0,2,
	//				8,4,4,4,4,4,4,4,4,7 };

	//map.loadMap(grid, sizeof(grid) / sizeof(int), 10, 10, "../../../resources/textures/wall.jpg", "../../../resources/textures/floor.jpg");

	int grid[] = {	5,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,6,
					1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,2,
					1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,2,
					1,0,0,0,9,0,0,0,0,0,0,0,0,0,0,0,0,0,0,2,
					1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,2,
					1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,2,
					1,0,0,0,0,0,0,0,0,0,0,0,0,9,0,0,0,0,0,2,
					1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,2,
					1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,2,
					1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,2,
					1,0,0,0,0,0,0,0,0,0,0,9,0,0,0,0,0,0,0,2,
					1,0,0,0,0,9,0,0,0,0,0,0,0,0,0,0,0,0,0,2,
					1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,2,
					1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,2,
					1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,2,
					1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,2,
					1,0,0,0,0,0,0,0,0,0,0,0,0,9,0,0,0,0,0,2,
					1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,2,
					1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,2,
					8,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,7 };


	map.loadMap(grid, sizeof(grid) / sizeof(int), 20, 20, "../../../resources/textures/wall.jpg", "../../../resources/textures/floor.jpg");
	addPlayer(-1.5f, -1.5f);
}

void Game::addPlayer(float x, float z)
{
	player = Player("awesomeDude", x, z);
}


bool Game::update()
{
	//Loading success flag
	bool success = true;

	timeStep = timer->getTicks()/10;
	player.updateVelocity();
	player.move(timeStep);

	timer->restart();
	return success;
}

void Game::render()
{
	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glm::mat4 model = glm::mat4(1.0f);

	glm::mat4 view = glm::mat4(1.f);
	view = glm::translate(view, glm::vec3(player.worldSpacePosition.x, player.worldSpacePosition.y, player.worldSpacePosition.z));
	
	glm::mat4 projection;
	projection = glm::perspective(glm::radians(60.0f), float(window_width) / float(window_height), 0.1f, 100.0f);
	projection = glm::rotate(projection, glm::radians(player.rotationDegrees) , glm::vec3(0.0f, 1.0f, 0.0f));

	lightShader.setView(view);
	lightShader.setProjection(projection);
	lightShader.setVector3("lightColour", glm::vec3(1.f, 1.f, 1.f));
	lightShader.setVector3("lightPos", glm::vec3(2.f, 1.f, 2.f));

	shader.setModel(model);
	shader.setView(view);
	shader.setProjection(projection);



	shader.setInt("pointLightSlotsFilled", pointLights.size());
	int i = 0;
	for (auto& light:pointLights.models) {
		shader.setVector3("pointLights[" + std::to_string(i) + "].ambient", glm::vec3(0.2f, 0.2f, 0.2f));
		shader.setVector3("pointLights[" + std::to_string(i) + "].diffuse", glm::vec3(1.f, 1.f, 1.f));
		shader.setVector3("pointLights[" + std::to_string(i) + "].position", light.second->worldPosition);
		shader.setFloat("pointLights[" + std::to_string(i) + "].constant", 1.0f);
		shader.setFloat("pointLights[" + std::to_string(i) + "].linear", 0.09f);
		shader.setFloat("pointLights[" + std::to_string(i) + "].quadratic", 0.032f);

		light.second->Draw(lightShader);
		i++;
	}


	map.Draw(shader);

	SDL_GL_SwapWindow(window);	
}

void Game::clean()
{
	SDL_GL_DeleteContext(context);

	//Destroy window
	SDL_DestroyWindow(window);
	window = NULL;

	//Quit SDL subsystems
	SDL_Quit();
}

bool Game::handleEvents()
{
	SDL_PollEvent(&event);
	switch (event.type) {
	case SDL_QUIT:
		isRunning = false;
		return true;
	case SDL_WINDOWEVENT:
		if (event.window.event == SDL_WINDOWEVENT_RESIZED) {
			glViewport(0, 0, event.window.data1, event.window.data2);
		}
		break;
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

