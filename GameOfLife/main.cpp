#include <iostream>
#include <vector>

#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>

#include "LifeGame.h"

#define NUM_STEPS	200
#define DEBUG_SIZE	30
#define GRAPHICS	1

//this should be in a different file?
void drawToSFML(sf::RenderWindow &window, std::vector<bool> grid, int size) {
	//clear display
	window.clear(sf::Color::Black);

	sf::RectangleShape block(sf::Vector2f(window.getSize().x / float(size), window.getSize().y / float(size)));
	for (int i = 0; i < size*size; i++)	{
		if (grid[i]) {
			block.setFillColor(sf::Color::Green);
			block.setPosition(sf::Vector2f(float(i%size) * window.getSize().x / float(size), float(i / size) * window.getSize().y / float(size)));
			window.draw(block);
		}
	}

	window.display();
}

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

		if (size > 1000) {
			std::cout << "Too large - graphics mode fails. Quitting." << std::endl;
			return 1;
		}
		else if (size > 250)
			std::cout << "Warning - this will likely be slow." << std::endl;
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
#ifdef GRAPHICS
	int curr_step = 0;
	sf::RenderWindow window;
	window.create(sf::VideoMode(1000,1000), "Game of Life", sf::Style::Close);
	//window.setSize(sf::Vector2u(4 * size, 3 * size));

	//draw initial state
	drawToSFML(window, game.current_buffer(), size);

	while(window.isOpen()) {
		sf::Event event;
		while(window.pollEvent(event)) {
			if (event.type == sf::Event::Closed) {
				window.close();
			}
			else if (event.type == sf::Event::KeyPressed) {
				if (curr_step++ == num_steps)
					window.close();

				//also do text because reasons
				//std::cout << std::endl << game.display() << std::endl;

				game.update();
				drawToSFML(window, game.current_buffer(), size);

				std::string title = "Game of Life - Step ";
				title.append(std::to_string(curr_step));
				window.setTitle(title);

			}
			else
				drawToSFML(window, game.current_buffer(), size);
		}
	}
#else
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
#endif
	return 0;
}