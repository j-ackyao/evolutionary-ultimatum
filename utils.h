#ifndef __utils_h__
#define __utils_h__

// random seed
#define SEED 8888

// max resources (positive double)
#define MAX_RESOURCE 10

// how many player-one-games a player does in each generation (positive int)
#define INTERACTIONS_PER_ITER 10

// whether players encountered by chance should be allowed to gain fitness (bool)
// if 0, the second player will not receive fitness that they accept
// if 1, the second player will receive the fitness they accept
#define SECONDARY_INTERACT 0

// top percentile of population to keep as survivors (0 to 1 double)
#define FITNESS_CUTOFF 0.5

// proportion of population at end of each iteration reserved for "mutation"/new randoms (0 to 1 double)
// the proportion of players directly from previous generations would be 1 - MUTATION_RATE
#define MUTATION_RATE 0.01

// whether survivors should remain in the next generation or only its children (bool)
// if the player's fitness allows for 5 and keep self is 1, player will have 4 children and itself in the next generation
#define KEEP_SELF 0

// whether survivors are allowed to reproduce with self (bool)
#define SELF_RPD 1


void assertions();

int read_int();

double get_rand();

double get_rand_range(double low, double high);

double min(double a, double b);

double max(double a, double b);

double avg(double a, double b);

#endif