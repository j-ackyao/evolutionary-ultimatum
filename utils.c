#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

#include "utils.h"


void assertions() {
	assert(MAX_RESOURCE > 0);
	assert(INTERACTIONS_PER_ITER > 0);
	assert(SECONDARY_INTERACT == 1 || SECONDARY_INTERACT == 0);
	assert(FITNESS_CUTOFF <= 1);
	assert(FITNESS_CUTOFF >= 0);
	assert(MUTATION_RATE + FITNESS_CUTOFF <= 1);
	assert(MUTATION_RATE >= 0);
	assert(KEEP_SELF == 1 || KEEP_SELF == 0);
	assert(SELF_RPD == 1 || SELF_RPD == 0);
}


int read_int() {
	int buf;
	int result = scanf("%d", &buf);
	if (result != 1) {
		printf("Invalid input or input stream error\n");
		return 0x80000000;
	}
	return buf;
}

// 0 to 1
double get_rand() {
	return rand() / (double) RAND_MAX;
}

// low to high, both inclusive
double get_rand_range(double low, double high) {
	return get_rand()*(high - low) + low;
}

double max(double a, double b) {
	return a > b ? a : b;
}

double min(double a, double b) {
	return a < b ? a : b;
}

double avg(double a, double b) {
	return (a + b) / 2;
}

