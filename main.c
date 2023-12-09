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

	if (!pop) {
		printf("Invalid population size provided\n");
		return 0;
	}

	printf("Creating an inital population of %d... ", pop);
	fflush(stdout);
	init_game(pop);
	printf("Done\n");

	summarize_game();

	clear_game();

	// printf("player offer: %f\nplayer lowerbound: %f\nplayer upperbound: %f\n", a.offer, a.lbound, a.ubound);
}

