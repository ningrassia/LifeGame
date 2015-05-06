#include <iostream>

#include "LifeGame.h"

#define NUM_STEPS 200
#define DEBUG_SIZE 30

/*
TODO:
fix init stuff
make it work.
*/

int main(int argc, char* *argv) {

	int size;
#ifdef _DEBUG
	size = DEBUG_SIZE;
#else
	//get size of grid
	if (argc < 2) {
		std::cout << "Please specify size of grid, and optionally start state. Quitting." << std::endl;
		return 1;
	}
	else  {
		size = atoi(argv[1]);
	}
#endif

	//make game instance
	LifeGame game(size);

#ifdef _DEBUG
	game.randomize();
#else
	//set up initial state
	if(argc == 2) {
		game.randomize();


	}
	else if (argc == 3) {
		if(!game.load_file(argv[2])){
			std::cout << "Error loading file " << argv[2] << std::endl;
			return 1;
		}
	}
#endif
	//run game
	std::cout << "Initial State" << std::endl << game.display() << std::endl;
	std::cout << "Press any key to step simulation. x to skip drawing." << std::endl << std::endl;
	getchar();
	for (int i = 0; i < NUM_STEPS; i++) {
		char in = getchar();
		game.update();
		if(in != 'x')
			std::cout << "Step: " << (i) << std::endl << game.display() << std::endl;
	}
	std::cout << "Simulation complete." << std::endl;
	getchar();
	return 0;
}