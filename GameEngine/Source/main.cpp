#include <memory>
#include "Game.h"

int main(int argc, char* args[])
{
	std::unique_ptr<Game> adventureGame = std::make_unique<Game>();

	return 0;
}