#include <iostream>

#include "LifeGame.h"

#define NUM_STEPS 200
#define DEBUG_SIZE 30

int main(int argc, char* *argv) {

	int size, num_steps;
#ifdef _DEBUG
	size = DEBUG_SIZE;
	num_steps = NUM_STEPS;
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
	else {
		if(!game.load_file(argv[2])){
			std::cout << "Error loading file " << argv[2]  << "." << std::endl;
			return 1;
		}
	}
#endif
	if (argc == 4)
		num_steps = atoi(argv[3]);
	else
		num_steps = NUM_STEPS;

	//run game
	std::cout << "Initial State" << std::endl << game.display() << std::endl;
	std::cout << "Press enter to step simulation. repeated x characters will skip drawing steps." << std::endl << std::endl;
	getchar();
	for (int i = 0; i < num_steps; i++) {
		char in = getchar();
		game.update();
		if(in != 'x')
			std::cout << "Step: " << (i+1) << std::endl << game.display() << std::endl;
	}
	std::cout << "Simulation complete." << std::endl;
	getchar();
	return 0;
}