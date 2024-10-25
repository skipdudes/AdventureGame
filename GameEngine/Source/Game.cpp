#include "Game.h"
#include "Globals.h"
#include "Constants.h"
#include "Logger.h"
#include "Levels/Menu.h"
#include "Levels/Exit.h"

Game::Game()
	:mWindow(nullptr), mFullscreen(false)
{
	//Initialize the game
	if (!init())
	{
		//Close subsystems upon error
		LOG_ERROR("Failed to initialize the game");
		close();
		return;
	}

	//Load the data
	if (!loadData())
	{
		//Free loaded data and close subsystems
		LOG_ERROR("Failed to load the data");
		freeData();
		close();
		return;
	}

	//Run the game loop
	run();
}

bool Game::init()
{
	LOG_INFO("Starting game initialization");

	//SDL
	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) < 0)
	{
		LOG_ERROR("SDL could not initialize, SDL Error: " + std::string(SDL_GetError()));
		return false;
	}

	//Window
	mWindow = SDL_CreateWindow(TITLE.c_str(), SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
	if (mWindow == nullptr)
	{
		LOG_ERROR("Window could not be created, SDL Error: " + std::string(SDL_GetError()));
		return false;
	}

	//Renderer
	gRenderer = SDL_CreateRenderer(mWindow, -1, SDL_RENDERER_ACCELERATED);
	if (gRenderer == nullptr)
	{
		LOG_ERROR("Renderer could not be created, SDL Error: " + std::string(SDL_GetError()));
		return false;
	}
	if (SDL_SetRenderDrawColor(gRenderer, 0x00, 0x00, 0x00, 0xFF) < 0)
		LOG_WARNING("Could not set renderer draw color, SDL Error: " + std::string(SDL_GetError()));
	if (SDL_RenderSetLogicalSize(gRenderer, SCREEN_WIDTH, SCREEN_HEIGHT) < 0)
		LOG_WARNING("Could not set renderer logical size, SDL Error: " + std::string(SDL_GetError()));

	//SDL_image
	int imgFlags = IMG_INIT_PNG;
	if (!(IMG_Init(imgFlags) & imgFlags))
	{
		LOG_ERROR("SDL_image could not initialize, SDL_image Error: " + std::string(IMG_GetError()));
		return false;
	}

	//SDL_mixer
	if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0)
	{
		LOG_ERROR("SDL_mixer could not initialize, SDL_mixer Error: " + std::string(Mix_GetError()));
		return false;
	}

	LOG_INFO("Finished game initialization");
	return true;
}

bool Game::loadData()
{
	LOG_INFO("Starting loading data");

	//Load global data
	gFontSmall = std::make_unique<Font>();
	if (!gFontSmall->build(FILE_FONT_SMALL_TEXTURE.string()))
	{
		LOG_ERROR("Could not load the small font");
		return false;
	}

	gFontMedium = std::make_unique<Font>();
	if (!gFontMedium->build(FILE_FONT_MEDIUM_TEXTURE.string()))
	{
		LOG_ERROR("Could not load the medium font");
		return false;
	}

	gFontLarge = std::make_unique<Font>();
	if (!gFontLarge->build(FILE_FONT_LARGE_TEXTURE.string()))
	{
		LOG_ERROR("Could not load the large font");
		return false;
	}

	gPlayer = std::make_unique<Player>();
	if (!gPlayer->load())
	{
		LOG_ERROR("Could not load the player");
		return false;
	}

	LOG_INFO("Finished loading data");
	return true;
}

void Game::run()
{
	LOG_INFO("Starting the game loop");

	SDL_Event e;
	gCurrentLevel = Menu::get();
	gCurrentLevel->enter();

	Uint32 lastFrameStartTime = SDL_GetTicks();
	Uint32 timeAccumulator = 0;

	while (gCurrentLevel != Exit::get())
	{
		//Calculate the time between the frames
		Uint32 currentFrameStartTime = SDL_GetTicks();
		Uint32 elapsedFrameTime = currentFrameStartTime - lastFrameStartTime;
		lastFrameStartTime = currentFrameStartTime;
		timeAccumulator += elapsedFrameTime;

		//Handle user input
		handleEvents(e);

		//Update with fixed time step
		while (timeAccumulator >= MAX_UPDATETIME)
		{
			update();
			timeAccumulator -= MAX_UPDATETIME;
		}

		//Render
		render();

		//Cap the framerate
		Uint32 frameTime = SDL_GetTicks() - currentFrameStartTime;
		if (frameTime < MIN_FRAMETIME)
			SDL_Delay(MIN_FRAMETIME - frameTime);
	}

	LOG_INFO("Finished the game loop");
}

void Game::handleEvents(SDL_Event& e)
{
	while (SDL_PollEvent(&e) != 0)
	{
		gCurrentLevel->handleEvents(e);

		if (e.type == SDL_QUIT)
			setNextState(Exit::get());
		else if ((e.type == SDL_KEYDOWN) && (e.key.keysym.sym == BUTTON_FULLSCREEN))
			toggleFullscreen();
	}
}

void Game::update()
{
	gCurrentLevel->update();
	changeState();
}

void Game::render()
{
	SDL_RenderClear(gRenderer);

	gCurrentLevel->render();

	SDL_RenderPresent(gRenderer);
}

void Game::toggleFullscreen()
{
	//Store mouse position
	int mouseX, mouseY;
	SDL_GetGlobalMouseState(&mouseX, &mouseY);

	//Change display mode
	mFullscreen = !mFullscreen;
	if (!mFullscreen)
	{
		LOG_INFO("Setting display mode to windowed");
		if (SDL_SetWindowFullscreen(mWindow, 0) < 0)
			LOG_WARNING("Could not set display to windowed mode, SDL Error: " + std::string(SDL_GetError()));
	}
	else
	{
		LOG_INFO("Setting display mode to fullscreen");
		if (SDL_SetWindowFullscreen(mWindow, SDL_WINDOW_FULLSCREEN_DESKTOP) < 0)
			LOG_WARNING("Could not set display to fullscreen mode, SDL Error: " + std::string(SDL_GetError()));
	}

	//Recover mouse position
	if (SDL_WarpMouseGlobal(mouseX, mouseY) < 0)
		LOG_WARNING("Mouse position could not be restored, SDL Error: " + std::string(SDL_GetError()));
}

void Game::freeData()
{
	LOG_INFO("Starting freeing data");

	//Free the data

	LOG_INFO("Finished freeing data");
}

void Game::close()
{
	LOG_INFO("Starting game teardown");

	//Window	
	SDL_DestroyRenderer(gRenderer);
	SDL_DestroyWindow(mWindow);
	gRenderer = nullptr;
	mWindow = nullptr;

	//SDL
	Mix_Quit();
	IMG_Quit();
	SDL_Quit();

	LOG_INFO("Finished game teardown");
}

Game::~Game()
{
	//Free the memory
	freeData();

	//Close subsystems
	close();
}