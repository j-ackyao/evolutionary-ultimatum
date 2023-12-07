#include <stdio.h>
#include <stdlib.h>
#include "utils.h"
#include "player.h"

static const int SEED = 8888;

int pop;
int gen;

int main(int argv, char* argc[]) {
	srand(SEED);

	if (argv == 1) {
		printf("Provide an initial population\n");
		return 0;
	} else if (argv == 2) {
		printf("Provide number of generations\n");
		return 0;
	}

	pop = atoi(argc[1]);
	gen = atoi(argc[2]);
	
	printf("Initial population size: %d\n", pop);
	for (int i = 0; i < 10; i++)
		init_player();

	struct player a = init_player();

	printf("%f", a.lbound);
}

