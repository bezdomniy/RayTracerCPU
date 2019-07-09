#include "Game.h"
#include <iostream>

SDL_Event Game::event;


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


bool Game::init(const char* title, int xpos, int ypos, bool fullscreen)
{
	//Initialization flag
	bool success = true;

	//Use OpenGL 3.1 core 
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
		
		window = SDL_CreateWindow(title, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, window_width, window_height, SDL_WINDOW_OPENGL);
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
	float vertices[] = {
		 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		 0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
		-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,

		 0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
		 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		-0.5f,  0.5f,  0.5f,  0.0f, 1.0f,

		 0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
		 0.5f,  0.5f, -0.5f,  1.0f, 0.0f,
		-0.5f,  0.5f, -0.5f,  0.0f, 0.0f,
		-0.5f,  0.5f,  0.5f,  0.0f, 1.0f,

		 0.5f,  -0.5f, 0.5f,  1.0f, 1.0f,
		 0.5f,  -0.5f,-0.5f,  1.0f, 0.0f,
		-0.5f,  -0.5f,-0.5f,  0.0f, 0.0f,
		-0.5f,  -0.5f, 0.5f,  0.0f, 1.0f,

		 0.5f,   0.5f, 0.5f,  1.0f, 1.0f,
		 0.5f,  -0.5f, 0.5f,  1.0f, 0.0f,
		 0.5f,  -0.5f,-0.5f,  0.0f, 0.0f,
		 0.5f,   0.5f,-0.5f,  0.0f, 1.0f,

		-0.5f,   0.5f, 0.5f,  1.0f, 1.0f,
		-0.5f,  -0.5f, 0.5f,  1.0f, 0.0f,
		-0.5f,  -0.5f,-0.5f,  0.0f, 0.0f,
		-0.5f,   0.5f,-0.5f,  0.0f, 1.0f

	};


	unsigned int indices[] = {  
		0, 1, 3,  
		1, 2, 3, 
		4, 5, 7,
		5, 6 ,7,
		8, 9, 11,
		9, 10,11,
		12, 13, 15,
		13, 14, 15,
		16, 17, 19,
		17, 18, 19,
		20, 21, 23,
		21, 22, 23,
		24, 25, 27,
		25, 26, 27
	};

	texture = Texture("../../../resources/textures/wall.jpg");
	createVAO(vertices, sizeof(vertices), indices, sizeof(indices));
}


bool Game::update()
{
	//Loading success flag
	bool success = true;

	timeStep = timer->getTicks();

	//timer->restart();
	return success;
}

void Game::render()
{
	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glm::mat4 model = glm::mat4(1.0f);
	model = glm::rotate(model, glm::radians(1.0f)*timeStep/10.f  , glm::vec3(0.5f, 1.0f, 0.0f));

	glm::mat4 view = glm::mat4(1.f);
	view = glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f));

	glm::mat4 projection;
	projection = glm::perspective(glm::radians(60.0f), float(window_width / window_height), 0.1f, 100.0f);

	glBindTexture(GL_TEXTURE_2D, texture.get());

	shader.use();

	unsigned int modelLoc = glGetUniformLocation(shader.id, "model");
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

	unsigned int viewLoc = glGetUniformLocation(shader.id, "view");
	glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));

	unsigned int projectionLoc = glGetUniformLocation(shader.id, "projection");
	glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));

	glBindVertexArray(VAO);
	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, (void*)0);
	glBindVertexArray(0);

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

void Game::createVAO(float vertices[], size_t sizeV, unsigned int indices[], size_t sizeI)
{
	glGenBuffers(1, &EBO);
	glGenBuffers(1, &VBO);
	glGenVertexArrays(1, &VAO);

	// 1. bind Vertex Array Object
	glBindVertexArray(VAO);
		// 2. copy our vertices array in a vertex buffer for OpenGL to use
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, sizeV, vertices, GL_STATIC_DRAW);
		// 3. copy our index array in a element buffer for OpenGL to use
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeI, indices, GL_STATIC_DRAW);
		// 4. then set the vertex attributes pointers
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(0);

		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
		glEnableVertexAttribArray(1);

	glBindVertexArray(0);
}





