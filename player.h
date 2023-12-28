#ifndef __player_h__
#define __player_h__

// max resources (positive double)
#define MAX_RESOURCE 10

// how many player-one-games a player does in each generation (positive int)
#define INTERACTIONS_PER_ITER 1

// the proportion of fitness a player must have in total fitness to reproduce (0 to 1 double)
#define FITNESS_CUTOFF 0.01

struct player {
	double offer;
	double lbound;
	double ubound;
	double fitness;
};

struct player init_player();

#endif