#ifndef __player_h__
#define __player_h__

// max resources (positive double)
#define MAX_RESOURCE 10

// how many player-one-games a player does in each generation (positive int)
#define INTERACTIONS_PER_ITER 10

// top percentile of population to keep (0 to 1 double)
#define FITNESS_CUTOFF 0.3

// proportion of population at end of each iteration reserved for "mutation"/new randoms (0 to 1 double)
#define MUTATION_RATE 0.1

// whether survivors should remain in the next generation or only its children (bool)
#define KEEP_SELF 1

struct player {
	double offer;
	double lbound;
	double ubound;
	double fitness;
};

struct player init_player();

#endif