#include <stdio.h>
#include <stdlib.h>

#include "utils.h"
#include "player.h"
#include "game.h"


static const int SEED = 8888;

int pop;
int gen;

int main(int argv, char* argc[]) {
	srand(SEED);

	printf("Provide an initial population: ");
	int pop = read_int();

	if (pop <= 1) {
		printf("Invalid population size provided\n");
		return 0;
	}

	printf("Creating an inital population of %d... ", pop);
	fflush(stdout);
	init_game(pop);
	printf("Done\n");
	printf("Input number of iterations to run with population, -1 to exit\n");
	int input;
	for (int input = read_int();;input = read_int()) {
		if (input == -2) 
			continue;
		else if (input < 0)
			break;
		
		printf("Running %d iterations...\n", input);

		run_sim_i(input);

		summarize_game();
	}

	printf("Exiting...\n");
	clear_game();

	// printf("player offer: %f\nplayer lowerbound: %f\nplayer upperbound: %f\n", a.offer, a.lbound, a.ubound);
}

