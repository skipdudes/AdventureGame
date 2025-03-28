#pragma once
#include <SDL.h>
#include <SDL_image.h>
#include <string>

class Game
{
public:
	Game();
	~Game();

private:
	SDL_Window* mWindow;
	SDL_Texture* mCurrentFrame;
	bool mFullscreen;

	bool init();
	bool loadData();
	void run();

	void handleEvents(SDL_Event& e);
	void update();
	void render();

	void toggleFullscreen();

	void freeData();
	void close();
};