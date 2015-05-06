#pragma once

#include <string>
#include <vector>
#include <array>
#include <random>
class LifeGame
{
public:
	LifeGame(int size);
	~LifeGame();

	void randomize();
	bool load_file(std::string filename);

	void update();
	bool set(int x, int y);
	bool clear(int x, int y);
	
	std::string display();
	std::vector<bool> current_buffer();
	
private:



	int get_neighbors(int x, int y);
	void update_cell(int cell, std::vector<bool> &from, std::vector<bool> &to);
	int size;
	bool curr_board; //false means board_a was last update, true means board_b was last updated.

	std::default_random_engine generator;
	std::vector<bool> board_a, board_b;
	std::vector<std::array<int, 8>> neighbors;
};

