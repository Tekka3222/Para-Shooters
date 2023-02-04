#include "Game.h"
#include <iostream>

int main(void) {
	Game game;
	bool success = game.Initialize();
	if (success) {
		game.RunLoop();
	}
	return 0;
}
