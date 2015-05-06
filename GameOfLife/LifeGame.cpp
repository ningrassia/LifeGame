#include "LifeGame.h"

#include <sstream>
#include <fstream>
#include <random>
#include <chrono>

LifeGame::LifeGame(int size) :size{ size }{
	board_a.resize(size * size);
	board_b.resize(size * size);
	neighbors.resize(size * size);

	std::chrono::system_clock clock;
	generator.seed(clock.to_time_t(clock.now()));

	curr_board = false;
	for (int i = 0; i < (size*size); i++) {
		board_a[i] = false;
		board_b[i] = false;

		int x, x_l, x_r, y, y_u, y_d;
		x = i%size;
		y = i / size;

		//find neighboring coordinates
		x_l = (x > 0) ? (x - 1) : (size - 1);
		x_r = ((x + 1) < size) ? (x + 1) : 0;
		y_u = (y > 0) ? (y - 1) : (size - 1);
		y_d = ((y + 1) < size) ? (y + 1) : 0;

		//gen points for neighbors
		neighbors[i][0] = x_l + (y_u * size);
		neighbors[i][1] = x_r + (y_u * size);
		neighbors[i][2] = x_l + (y_d * size);
		neighbors[i][3] = x_r + (y_d * size);
		neighbors[i][4] = x_l + (y * size);
		neighbors[i][5] = x_r + (y * size);
		neighbors[i][6] = x + (y_u * size);
		neighbors[i][7] = x + (y_d * size);
	}
}


LifeGame::~LifeGame(){

}


//RULES from wikipedia
/*
Any live cell with fewer than two live neighbours dies, as if caused by under-population.
Any live cell with two or three live neighbours lives on to the next generation.
Any live cell with more than three live neighbours dies, as if by overcrowding.
Any dead cell with exactly three live neighbours becomes a live cell, as if by reproduction.
*/
void LifeGame::update(){
	int i;
	for (i = 0; i < (size*size); i++) {
		if (curr_board)
			update_cell(i, board_b, board_a);
		else
			update_cell(i, board_a, board_b);
	}
	curr_board = !curr_board; //swap the buffer!
}

bool LifeGame::set(int x, int y){
	if ((x > size) || (y > size))
		return false;
	if (curr_board)
		board_b[x + (y*size)] = true;
	else
		board_a[x + (y*size)] = true;

	return true;
}

bool LifeGame::clear(int x, int y){
	if ((x > size) || (y > size))
		return false;
	if (curr_board)
		board_b[x + (y*size)] = false;
	else
		board_a[x + (y*size)] = false;
	return true;
}

std::string LifeGame::display(){
	int x, y;
	std::ostringstream os;
	for (y = 0; y < size; y++) {
		for (x = 0; x < size; x++) {
			if (curr_board) {
				if (board_b[x + (y*size)])
					os << "X";
				else
					os << ".";
			}
			else {
				if (board_a[x + (y*size)])
					os << "X";
				else
					os << ".";
			}

		}
		os << std::endl;
	}
	return os.str();
}

std::vector<bool> current_buffer() {
	return curr_board ? board_b : board_a;
}

void LifeGame::randomize() {
	std::uniform_int_distribution<int> num_rolls_distribution(0, (size * size) - 1);
	std::uniform_int_distribution<int> coordinate_distribution(0, size - 1);

	int num_rolls = num_rolls_distribution(generator);
	for (int i = 0; i < num_rolls; i++) {
		this->set(coordinate_distribution(generator), coordinate_distribution(generator));
	}

}

//TODO: make me suck less
bool  LifeGame::load_file(std::string filename) {
	std::fstream file(filename, std::fstream::in);
	//if file is bad return false;
	if (file.fail())
		return false;
	int i = 0;
	while (i < (size * size)) {
		char next;
		//if we don't have enough info return!
		if (file.eof())
			return false;
		next = file.get();
		if (next == 'X') {
			this->set(i % size, i / size);
			i++;
		}
		else if (next == '.') {
			this->clear(i % size, i / size);
			i++;
		}
	}
	return true;
}

int LifeGame::get_neighbors(int x, int y){
	int num_neighbors = 0;
	//check points
	if (curr_board) {
		for (int i = 0; i < 8; i++) {
			if (board_b[neighbors[x + (y*size)][i]])
				num_neighbors++;
		}
	}
	else {
		for (int i = 0; i < 8; i++) {
			if (board_a[neighbors[x + (y*size)][i]])
				num_neighbors++;
		}
	}
	return num_neighbors;
}

void LifeGame::update_cell(int cell, std::vector<bool> &from, std::vector<bool> &to) {
	int neighbors = get_neighbors(cell%size, cell / size);
	if (from[cell]) {
		if (neighbors < 2)
			to[cell] = false;
		else if (neighbors > 3)
			to[cell] = false;
		else
			to[cell] = true;
	}
	else {
		if (neighbors == 3)
			to[cell] = true;
		else
			to[cell] = false;
	}
}
