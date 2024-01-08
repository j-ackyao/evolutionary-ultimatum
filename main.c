#include <stdio.h>
#include <stdlib.h>

#include "utils.h"
#include "player.h"
#include "game.h"


int main(int argv, char* argc[]) {
	srand(SEED);

	// assertion checks to ensure hyperparameters are valid
	assertions();

	printf("Provide an initial population: ");
	int pop = read_int();

	if (pop <= 1) {
		printf("Invalid population size provided\n");
		return 0;
	}

	printf("Creating an inital population of %d", pop);
	fflush(stdout);
	init_game(pop);
	printf(" Done\n");

	summarize_game(0);
	printf("Input number of iterations to run with population,\n-1 to exit,\n-2 to toggle writing to CSV\n");
	int input;

	int iterations = 0;
	int write = 0;
	for (int input = read_int();; input = read_int()) {
		if (input == -2) {
			write = write ? 0 : 1;
			printf("%s writing to CSV file\n", write ? "Now" : "Stopped");
			continue;
		} else if (input < 0) {
			break;
		}
		
		printf("Running %d iterations", input);
		fflush(stdout);

		iterations += input;

		run_sim_i(input);

		summarize_game(iterations);

		if (write)
			write_csv(iterations);
	}

	printf("Exiting...\n");
	clean();
}

